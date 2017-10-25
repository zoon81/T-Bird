#ifndef INC_SEVSEG_H
#define INC_SEVSEG_H
    #include "main.h"

    void sevseg_init();
    void sevseg_putvalue(uint8_t digit, uint8_t value);
    void sevseg_putnumber_16(uint16_t number);
    
#endif