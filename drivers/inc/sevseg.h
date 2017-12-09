#ifndef INC_SEVSEG_H
#define INC_SEVSEG_H
    #include "main.h"
    #define sevseg_blank_digit 10

    typedef enum {
        LEFT,
        RIGTH
    } sevseg_position_t;

    typedef enum{
        CLEAR,
        SET,
        TOOGLE
    } sevseg_led_operation_t;


    void sevseg_init();
    void sevseg_putvalue(uint8_t digit, uint8_t value);
    void sevseg_putnumber_16(uint16_t number);
    void sevseg_setdigit(uint8_t digit, uint8_t value);
    void sevseg_putnumber_8(uint8_t number, sevseg_position_t position);
    void sevseg_changeled(sevseg_led_operation_t command);
    
#endif