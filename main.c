#include "main.h"
#include <stdlib.h>

void start_ledshift();
void sevseg();
void keylock();
void show_data_lcd();

volatile uint8_t ledvalue = 0;
volatile uint8_t value = 0;
volatile uint8_t mkey = 0;
volatile uint8_t lockstatus = 0;

void main()
{
    keyboardmatrix_init();
    LCD_Init();
    sevseg_init();

    //TIMER3 PWM, Phase Correct, 8-bit
    //OCR3A = 127;
    //TCCR3A |= BIT(WGM30) | BIT(COM3A1);
    //TCCR3B |= BIT(CS32);   //  256 prescale
    //DDRE |= BIT(PE3);
    //uint8_t buffer[7];
    //uint16_to_str(buffer, 123456);
    //LCD_String_xy(2, 0, buffer);
    //LCD_String_xy(0,0, "PWM value(0-255)");
    
    /* while (1)
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
    } */
    DDRG = 0x00;
    //PORTG = 0x0F;
    //start_ledshift();
    while(1){
        uint8_t pinvalue = (PING & 0x0F);
        switch(pinvalue){
            case 1:             // button K0
                start_ledshift();
                break;
            case 2:             // button K1
                sevseg();
                break;
            case 4:             // button K2
                keylock();
                break;
            case 8:             // button K3
                show_data_lcd();
            default: break;
        }
    } 
}

void start_ledshift(){
    DDRB = 0xF0;
    DDRD = 0xF0;
    ledvalue = 0x03;
    uint8_t dir = 0;
    uint8_t counter = 30;
    while(counter--){
        PORTB = (ledvalue & 0x0F) << 4;
        PORTD =  (ledvalue & 0xF0);
        _delay_ms(500);
        if(ledvalue == 0xC0){
            dir = 1;
        }
        if(ledvalue == 0x03){
            dir = 0;
        }
        if(dir == 1){
            ledvalue >>= 1;
        }else{
            ledvalue <<= 1;
        }
    }
}
void sevseg(){
    value = 11;
    uint8_t counter = 130;
    while(counter--){
        sevseg_putnumber_8(value, 1);
        value++;
        if(value > 33){
            value = 11;
        }
        _delay_ms(200);
    }
}
void keylock(){
    sevseg_setdigit(2, sevseg_blank_digit);
    sevseg_setdigit(3, sevseg_blank_digit);
    DDRE |= BIT(3);
    DDRC |= BIT(7);
    mkey;
    char key[3];
    key[0] = (char) keyboardmatrix_getchar();
    sevseg_setdigit(3, key[0]);
    key[1] = (char) keyboardmatrix_getchar();
    sevseg_setdigit(2, key[1]);
    key[2] = 0;
    mkey = atoi(key);
    if(mkey == 66){
        PORTC &= ~BIT(7);
        PORTE |= BIT(3);
        lockstatus = 1;
    }else{
        PORTE &= ~BIT(3);
        PORTC |= BIT(7);
        lockstatus = 0;
    }
}
void show_data_lcd(){
    char tmp_str[5];
    LCD_String_xy(0, 0, "Ban Zoltan");
    uint8_to_str(tmp_str, ledvalue);
    LCD_String_xy(1, 0, tmp_str);
    uint8_to_str(tmp_str, mkey);
    LCD_String_xy(2, 0, tmp_str);
    if(lockstatus){
        LCD_Clear_xy(3,0,15);
        LCD_String_xy(3, 0, "VALID");
    }else{
        LCD_Clear_xy(3,0,15);
        LCD_String_xy(3, 0, "INVALID");
    }

}