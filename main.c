#include "main.h"

void main(){

    DDRB = 0xF0;
    sevseg_init();
    keyboardmatrix_init();
    sevseg_putnumber_8(50, LEFT);
    sevseg_putnumber_8(75, RIGTH);
    LCD_Init();
    LCD_String_xy(0, 0, "HELLO");
    char buffer[6];
    uint8_to_str(&buffer, 112);
    LCD_String(&buffer);
    while(1){
        char c[6];
        LCD_String_xy(1, 0, "STR: ");
        uint16_t numb = keyboardmatrix_getint_16(0);     //passing a function as a pointer here
        uint8_to_str(&c, (uint8_t) numb);
        LCD_String_xy(2, 0, "INT: ");
        LCD_String(&c);
        _delay_ms(100);
    }
}