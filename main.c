#include "main.h"

struct portstate_s portstate_lookup[] = {
    {0b10001001, 0b00100100, 0b01000100, 0b00000000, 1},
    {0b10011001, 0b00100101, 0b01000100, 0b00000000, 2},
    {0b01100001, 0b00100110, 0b01001000, 0b00000000, 5},
    {0b01010001, 0b00100101, 0b01001000, 0b00000000, 2},
    {0b10001011, 0b00100100, 0b01000100, 0b00000000, 2},
    {0b10001100, 0b00100100, 0b01000100, 0b00000001, 5},
    {0b10001010, 0b00100100, 0b01000100, 0b00000001, 2},
    {0b10001001, 0b01101100, 0b01000100, 0b00000000, 1},
    {0b10001001, 0b10010000, 0b10000100, 0b00000000, 5},
    {0b10001001, 0b01001000, 0b10000100, 0b00000000, 2}};
uint8_t state, systick;

void main()
{
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRE = 0xCC;
    DDRF = 0x0F;
    state = 0;
    systick = 0;
    // CTC mode with 1024 prescaller, irq on every 500ms
    OCR1A = 7812;
    TCCR1B = BIT(WGM12) | BIT(CS12) | BIT(CS10);
    TIMSK = BIT(OCIE1A);
    while(1){
        
    }
}

ISR(TIMER1_CMPA_vect){
    PORTA = portstate_lookup[state].port_a;
    PORTC = portstate_lookup[state].port_c;
    PORTE = portstate_lookup[state].port_e;
    PORTF = portstate_lookup[state].port_f;
    systick++;
    if(systick >= portstate_lookup[state].period * 2){
        state++;
        systick = 0;
    }
    if(state > 10){
        state = 0;
    }
}