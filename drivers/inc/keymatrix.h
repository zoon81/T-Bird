#ifndef INC_KEYMATRIX_H
#define INC_KEYMATRIX_H
#include "main.h"

#define KMPORT PORTC
#define KMPIN PINC
#define KMDDR DDRC
#define asterisk 10
#define hashmark 11
#define KBCOLLUMN_MASK 0x78

void keyboardmatrix_init();
uint8_t keyboardmatrix_read();
uint8_t keyboardmatrix_getchar();
uint16_t keyboardmatrix_getint_16( void (*LCD_sendbyte)(uint8_t) );
void keyboardmatrix_getstring(uint8_t *chr, uint8_t lenght);

#endif