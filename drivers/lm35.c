#include "lm35.h"

#define MAGIC_MULT 488 // 5000/1024*10  VRef / adcRES *10

uint16_t lm35_getTemperature(){
    //Drop first 2 value
    uint32_t mvolt = adc_getConversion(LM35_ADC_CHANNEL);
    mvolt = adc_getConversion(LM35_ADC_CHANNEL);
    mvolt = 0;
    //Avarage out 3 result
    mvolt += adc_getConversion(LM35_ADC_CHANNEL);
    mvolt += adc_getConversion(LM35_ADC_CHANNEL);
    mvolt += adc_getConversion(LM35_ADC_CHANNEL);
    mvolt = mvolt / 3;

    uint16_t temp;
    mvolt = mvolt * MAGIC_MULT;
    if(mvolt % 100 >= 50)
        temp = mvolt / 100 + 1;
    else
        temp = mvolt / 100;
    
    if(temp % 10 >= 7){
        temp = temp / 10 * 10 + 10;
    } else if(temp % 10 >= 3){
        temp = temp / 10 *10 + 5;
    } else{
        temp = temp / 10 * 10;
    }
    return temp;
}