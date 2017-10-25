#include "sevseg.h"


uint8_t values[4];
uint8_t index = 0;

void sevseg_init(){
    DDRA = 0xff;

    //TIMER0 CTC mode, IRQ on every ~3.2ms
    OCR0  = 50;
    TCCR0 = BIT(WGM01) | (7 << CS00);
    TIMSK = BIT(OCIE0);
    sei();

}
void sevseg_putvalue(uint8_t digit, uint8_t value){
    PORTA = (value & 0x0F) | (digit << 4) | (1 << 7);
}

void sevseg_putnumber_16(uint16_t number){
    values[0] = number % 10 ;
    values[1] = ((number /   10) % 10);
    values[2] = ((number /  100) % 10);
    values[3] = ((number / 1000) % 10);
}
//updating each digits in every 3.2ms = 1 digit updated about every 13ms
ISR(TIMER0_COMP_vect){
    if(index > 3 )
        index = 0;
    sevseg_putvalue(index, values[index]);
    index++;
    
}
