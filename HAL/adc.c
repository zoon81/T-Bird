#include "adc.h"

void adc_init(){
    // 128 Prescaler
    ADCSRA = BIT(ADEN) | 3;
    ADMUX = BIT(REFS0);
}

uint16_t adc_getConversion(uint8_t mux){
    ADMUX |= (ADMUX & 0xE0) | mux;
    ADCSRA |= BIT(ADSC);
    while( !(ADCSRA & BIT(ADSC)) );
    uint8_t adclow = ADCL;
    uint16_t adcdr = ADCH << 8 | adclow;
    return adcdr;
}