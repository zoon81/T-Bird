#include "sevseg.h"

// We can store values even only 2byte | digits only form 0 to 9, this can be repesented on 4bit. uint8_t 8bit variable = lower 4bit=digit0 upper 4bit=digit1
uint8_t values[4] = {sevseg_blank_digit, sevseg_blank_digit, sevseg_blank_digit, sevseg_blank_digit}; //start values for blank digits
uint8_t led_status = 0;
uint8_t index = 0;

void sevseg_init()
{
    DDRA = 0xff;

    //TIMER0 CTC mode, IRQ on every ~2.56ms
    OCR0 = 40;
    TCCR0 = BIT(WGM01) | (7 << CS00);
    TIMSK = BIT(OCIE0);
    sei();
}
void sevseg_setdigit(uint8_t digit, uint8_t value){
    values[digit] = value;
}
void sevseg_putvalue(uint8_t digit, uint8_t value)
{
    PORTA = (value & 0x0F) | (digit << 4) | (1 << 7);
}

void sevseg_putnumber_16(uint16_t number)
{
    values[0] = number % 10;
    values[1] = ((number / 10) % 10);
    values[2] = ((number / 100) % 10);
    values[3] = ((number / 1000) % 10);
}
void sevseg_putnumber_8(uint8_t number, sevseg_position_t position)
{
    if (position)
    {
        values[0] = number % 10;
        values[1] = ((number / 10) % 10);
    }
    else
    {
        values[2] = number % 10;
        values[3] = ((number / 10) % 10);
    }
}
void sevseg_changeled(sevseg_led_operation_t command)
{
    switch (command)
    {
    case 0:
        led_status = 0;
        break;

    case 1:
        led_status = 1;
        break;

    case 2:
        led_status = ~led_status;
        break;

    default:
        break;
    }
}

//updating each digits in every 2.56ms = 1 digit updated about every 12.8ms
ISR(TIMER0_COMP_vect)
{
    if (index == 4)                 //if we are finished the digits updating
    {
        if(led_status)              // and we set the led ON
            sevseg_putvalue(4, 0);  // set the multiplexer to driving LED
        else
            sevseg_putvalue(5, 0);  // If we set the LED OFF, we need to step forward the muliplexer in order to stop driving the last digit.This is solving the last digit flickering when the led is set ON.
        index = 0;
    }
    else                            // Updating the digit until reaching the last digit
    {
        sevseg_putvalue(index, values[index]);
        index++;
    }
    
}
