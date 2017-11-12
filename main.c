#include "main.h"

void main()
{
    // cake form 1
    uint8_t pixmap[8] = {
        0b10100,
        0b00000,
        0b01010,
        0b01010,
        0b01010,
        0b11111,
        0b11111,
        0b11111};
    // cake form 2
    uint8_t pixmap2[8] = {
        0b00101,
        0b00000,
        0b01010,
        0b01010,
        0b01010,
        0b11111,
        0b11111,
        0b11111};
    // ö char
    uint8_t customChar1[8] = {
        0b01010,
        0b00000,
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01110};
    // í char
    uint8_t customChar2[8] = {
        0b00010,
        0b00100,
        0b00000,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b01110};
    keyboardmatrix_init();
    LCD_Init();
    LCD_setCustomChar(0, pixmap);
    LCD_setCustomChar(1, pixmap2);
    LCD_setCustomChar(2, customChar1);
    LCD_setCustomChar(3, customChar2);
    LCD_String_xy(0, 4, "\0");
    LCD_Data(2);
    LCD_Data(3);
    //TIMER3 IRQ every 1s
    OCR3A = 7812;
    TCCR3B = BIT(CS30) | BIT(CS32) | BIT(WGM32); //1024 prescale, CTC
    OCR3A = 100;
    TIMSK = BIT(TOIE1);
    sei();
    //uint8_t buffer[7];
    //uint16_to_str(buffer, 123456);
    //LCD_String_xy(2, 0, buffer);
    
    uint8_t i;
    while (1)
    {
        LCD_String_xy(3, 0, "");
        i = 16;
        while (i--)
            LCD_Data(0);
        _delay_ms(800);
        LCD_String_xy(3, 0, "");
        i = 16;
        while (i--)
            LCD_Data(1);
        _delay_ms(800);
    }
}