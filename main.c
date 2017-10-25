#include "main.h"

void main(){
    DDRB = 0xF0;
    sevseg_init();
    sevseg_putnumber_16(4321);

    while(1){
        PORTB |= (1 << 4);
        _delay_ms(500);
        PORTB &= ~(1 << 4);
        _delay_ms(500);
    }
}