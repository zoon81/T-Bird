#include "main.h"

void main(){

    DDRB = 0xF0;
    sevseg_init();
    sevseg_putnumber_8(50, LEFT);
    sevseg_putnumber_8(75, RIGTH);
    LCD_Init();
    LCD_String_xy(0, 0, "HELLO");
    LCD_String_xy(1, 0, "MI");
    LCD_String_xy(2, 0, "LINE3");
    LCD_String_xy(3, 0, "LINE4");

    while(1){
        sevseg_changeled(TOOGLE);
        _delay_ms(500);
    }
}