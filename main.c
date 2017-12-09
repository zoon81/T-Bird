#include "main.h"

void main()
{
    keyboardmatrix_init();
    LCD_Init();

    //TIMER3 PWM, Phase Correct, 8-bit
    OCR3A = 127;
    TCCR3A |= BIT(WGM30) | BIT(COM3A1);
    TCCR3B |= BIT(CS32);   //  256 prescale
    DDRE |= BIT(PE3);
    //uint8_t buffer[7];
    //uint16_to_str(buffer, 123456);
    //LCD_String_xy(2, 0, buffer);
    LCD_String_xy(0,0, "PWM value(0-255)");
    
    while (1)
    {
        LCD_Clear_xy(2,0,15);
        LCD_String_xy(2,0, "Current val:");
        char ocr_value[4];
        uint8_to_str(ocr_value, (uint8_t) OCR3A);
        LCD_String(ocr_value);
        LCD_Clear_xy(3,0,15);
        LCD_String_xy(3,0,"New value: ");
        uint8_t new_ocr_val = keyboardmatrix_getint_16(LCD_Data);
        if(new_ocr_val < 255){
            OCR3A = new_ocr_val;
        }
    }
}