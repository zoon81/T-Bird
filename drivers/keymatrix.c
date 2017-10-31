#include "keymatrix.h"

void keyboardmatrix_init()
{
    KMDDR = KBCOLLUMN_MASK; //PC6-PC3 output, this pins are the collumn drivers
}
uint8_t keyboardmatrix_read()
{
    uint8_t result, collumn_index = 3;
    do
    {
        KMPORT = (1 << collumn_index++); //set collumn1 to active
        _delay_ms(5);                    //transient effect passed
        result = KMPIN & 0x7F;           //we need only the lower 7bit
        if (collumn_index > 6)           //rewinding collumn_index
            collumn_index = 3;
    } while ((result & 0x07) == 7);
    while ((KMPIN & 0x07) != 7)
        ;
    _delay_ms(50);
    //keys are active low,so while nothing be pushed, the readback give all key value as logic 1
    return result;
}

void keyboardmatrix_getstring(uint8_t *chr, uint8_t lenght)
{
    uint8_t scancodes[] = {0x45, 0x0E, 0x0D, 0x0B, 0x16, 0x15, 0x13, 0x26, 0x25, 0x23, 0x46, 0x43};
    uint8_t index = 12;
    while (lenght--)
    {
        uint8_t scancode = keyboardmatrix_read();
        while (index--)
        {
            if (scancode == scancodes[index])
            {
                if (index == 10)
                    *chr = 0x2A; //asterisk ascii code
                else if (index == 11)
                    *chr = 0x23; //hash mark ascii code
                else
                    *chr = index + 48;
                chr++;
            }
        }
        index = 12;
    }
}
uint8_t keyboardmatrix_getchar()
{
    uint8_t scancodes[] = {0x45, 0x0E, 0x0D, 0x0B, 0x16, 0x15, 0x13, 0x26, 0x25, 0x23, 0x46, 0x43};
    uint8_t index = 12;
    uint8_t c;
    uint8_t scancode = keyboardmatrix_read();
    while (index--)
    {
        if (scancode == scancodes[index])
        {
            if (index == 10)
                c = 0x2A; //asterisk ascii code
            else if (index == 11)
                c = 0x23; //hash mark ascii code
            else
                c = index + 48;
        }
    }
    return c;
}
// terminating char :  #
//This function get another function as a parameter, this parameter is designed to be a function which can printing charachters on the LCD
//You can pass a null (0) as a function argument, in this case the function will not able to printing on the LCD
uint16_t keyboardmatrix_getint_16( void (*LCD_sendbyte)(uint8_t) )
{
    uint8_t len = 0;
    uint8_t c;
    uint8_t str[6];
    uint16_t number=0;
    uint16_t multiplier = 1;

    do
    {
        c = keyboardmatrix_getchar();
        if (c < 0x3A && c > 0x2F){
            str[len++] = c;
            if(LCD_sendbyte){
                (*LCD_sendbyte)(c);
            }
        }
    } while (c != '#' && len < 6);

    do{
        number += multiplier * (str[--len] - 48 );
        multiplier *= 10;
    }while(len);

    return number;
}