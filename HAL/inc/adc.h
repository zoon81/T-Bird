#ifndef INC_ADC_H
#define INC_ADC_H

#include "main.h"

void adc_init();
uint16_t adc_getConversion(uint8_t mux);

#endif