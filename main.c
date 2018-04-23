#include "main.h"

#define SYSTICK_DIVIDER 4
#define STATES 10
#define SERVICE_STATE 10

// @ToDo :  BUG -- tc set rw lamps too when waiting for state 8                                 DONE
//          BUG -- rw lamp stay red while stepstate() not called after the TRAIN_GONE state     DONE

void stepState();

struct portstate_s portstate_lookup[] = {
    {0b10001001, 0b00100100, 0b01000100, 0b00000000, 1, { 0, {} } },
    {0b10011001, 0b00100101, 0b01000100, 0b00000000, 2, { 0, {} } },
    {0b01100001, 0b00100110, 0b01001000, 0b00000000, 5, { 0, {} } },
    {0b01010001, 0b00100101, 0b01001000, 0b00000000, 2, { 2, { {&PORTA, PA6}, {&PORTE, PE3} } } },
    {0b10001011, 0b00100100, 0b01000100, 0b00000000, 2, { 0, {} } },
    {0b10001100, 0b00100100, 0b01000100, 0b00000001, 5, { 0, {} } },
    {0b10001010, 0b00100100, 0b01000100, 0b00000001, 2, { 1, {{&PORTF, PF0}} } },
    {0b10001001, 0b01101100, 0b01000100, 0b00000000, 1, { 0, {} } },
    {0b10001001, 0b10010000, 0b10000100, 0b00000000, 5, { 0, {} } },
    {0b10001001, 0b01001000, 0b10000100, 0b00000000, 2, { 1, {{&PORTE, PE7}} } },
    {0x00, 0x00, 0x00, 0x00, 1, {5, { {&PORTC, PC0}, {&PORTC, PC3}, {&PORTA, PA1}, {&PORTA, PA4}, {&PORTC, PC6}}}}
};

volatile uint8_t state, systick, mode;

void main()
{
    uint8_t debug_mode = 0;
    UART0_Init(BAUD_9600);
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRE = 0xCC;
    DDRF = 0x0F;
    
    //Pullup for switches
    PORTE = BIT(RW_SENSE_LEFT) | BIT(RW_SENSE_RIGHT);

    state = 0;                                                      //State of the traffic lamps
    systick = 0;                                                    //Serious timer function, just like an RTOS
    mode = BIT(MODE_TRAIN_GONE);                                    //Default mode

    if (PING & BIT(SW_K2))                                         //Debug mode check, If this mod is set, we can steping trought the lookup table via K2 switch
    {
        debug_mode = 1;
    }

    // CTC mode with 1024 prescaller, irq on every ~250ms on COMPA and every ~100ms on COMPB
    // COMPA handling all the lamps and blinks, COMPB handling all the switches
    OCR1A = 3906;
    OCR1B = 1562;
    TCCR1B = BIT(WGM12) | BIT(CS12) | BIT(CS10);
    if (debug_mode)                                                 //We only enable COMPB interupt on debug mode
    {
        TIMSK = BIT(OCIE1B);
    }
    else
    {
        TIMSK = BIT(OCIE1A) | BIT(OCIE1B);
    }
    sei();
    stepState();                                                // Starting routine, ISR will handling all other
    while (1)
    {
        //Sense from left
        if(mode & BIT(MODE_TRAIN_FROM_LEFT)){
            PORTF &= ~BIT(RW_WHITE);                                                                //Turn White OFF
            mode |= BIT(MODE_TRAIN_RED);                                                             //Turing RW RED ON
            while((state != TC_RAILWAY_CLOSED_STATE) & !(mode & BIT(MODE_TRAIN_FROM_RIGHT)));           //Waiting while the tc reach this state
            mode &= ~BIT(MODE_TRAIN_GONE);                                                           //Pause tc at this state
            while( !(mode & BIT(MODE_TRAIN_FROM_RIGHT)));                                             //Wait for the train while reaching the oposite sense switch
            mode &= BIT(MODE_SERVICE) | BIT(MODE_SERVICE_REQUEST);                                   //Start tc again
            mode |= BIT(MODE_TRAIN_GONE);
            PORTF &= ~ (BIT(RW_RED_1) | BIT(RW_RED_2));                                             //Turn RED OFF
        }
        //Sense from right
        if(mode & BIT(MODE_TRAIN_FROM_RIGHT)){
            PORTF &= ~BIT(RW_WHITE);                                                                //Turn White OFF
            mode |= BIT(MODE_TRAIN_RED);                                                             //Turing RW RED ON
            while((state != TC_RAILWAY_CLOSED_STATE) & !(mode & BIT(MODE_TRAIN_FROM_LEFT)));            //Waiting while the tc reach this state
            mode &= ~BIT(MODE_TRAIN_GONE);                                                              //Pause tc at this state
            while( !(mode & BIT(MODE_TRAIN_FROM_LEFT)));                                                //Wait for the train reach the oposite sense switch
            mode &= BIT(MODE_SERVICE) | BIT(MODE_SERVICE_REQUEST);                                       //Start tc again (MASK SERVICE bits)
            mode |= BIT(MODE_TRAIN_GONE);
            PORTF &= ~ (BIT(RW_RED_1) | BIT(RW_RED_2));                                                 //Turn RED OFF
        }
        //Service mode
        if(mode & BIT(MODE_SERVICE_REQUEST)){                                                       //Turn ON service mode
            if((mode & BIT(MODE_TRAIN_GONE)) && (state == 0)){
                state = SERVICE_STATE;
                stepState();
                mode |= BIT(MODE_SERVICE);
            }
        } else if(mode & BIT(MODE_SERVICE)){                                                        //Turn OFF service mode
            state = 0;
            systick = 0;
            mode &= ~BIT(MODE_SERVICE);
            stepState();
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    // blink for trafic controll lamps
    if(portstate_lookup[state].blinkstate.blinking_pins > 0){
        uint8_t i;
        for(i = 0; i < portstate_lookup[state].blinkstate.blinking_pins; i++){
            *(portstate_lookup[state].blinkstate.pindesc[i].port) ^= (1 << portstate_lookup[state].blinkstate.pindesc[i].pin);
        }
    }
    // blink for railway
    if(mode & BIT(MODE_TRAIN_RED)){
        PORTF ^= BIT(RW_RED_1);
        if(PORTF & BIT(RW_RED_1)){
            PORTF &= ~(BIT(RW_RED_2));
        }else {
            PORTF |= BIT(RW_RED_2);
        }
    } else {
        PORTF ^= BIT(RW_WHITE);
    }
    //tc control
    if( (mode & BIT(MODE_TRAIN_GONE)) && !(mode & BIT(MODE_SERVICE))){
        systick++;
        if (systick >= portstate_lookup[state].period * SYSTICK_DIVIDER){
            state++;
            if (state > (STATES - 1) ){
             state = 0;
            }
            stepState();
            systick = 0;
        }
    }
}

void stepState(){

    PORTA = portstate_lookup[state].port_a;
    PORTC = portstate_lookup[state].port_c;
    PORTE = portstate_lookup[state].port_e | BIT(RW_SENSE_LEFT) | BIT(RW_SENSE_RIGHT);              //This for SW Pullups 
    PORTF = (portstate_lookup[state].port_f | (PORTF & BIT(RW_RED_1)) | (PORTF & BIT(RW_RED_2)) );  //RW red lamp unifying
    
}

ISR(TIMER1_COMPB_vect)
{

    if( PING & BIT(SW_K1) ){
        stepState();
        UART0_sendString("\n\rState:");
        char buff[4];
        uint8_to_str(buff, state);
        UART0_sendString(buff);
        state++;
        if (state > (STATES - 1))
        {
            state = 0;
        }
        while( PING & BIT(SW_K1) );   //wait until the user release the button
    }
    if( !(PINE & BIT(RW_SENSE_LEFT)) ){
        mode |= BIT(MODE_TRAIN_FROM_LEFT);
    }
    if( !(PINE & BIT(RW_SENSE_RIGHT)) ){
        mode |= BIT(MODE_TRAIN_FROM_RIGHT);
    }
    if( PING & BIT(SW_K0) ){
        //mode ^= BIT(MODE_SERVICE_REQUEST);                                            //Bad idea
        if(mode & BIT(MODE_SERVICE)){
            mode &= ~BIT(MODE_SERVICE_REQUEST);                                         //SET NORMAL MODE REQUEST
        } else {
            mode |= BIT(MODE_SERVICE_REQUEST);                                          //SET Service mode request
        }
    }
}