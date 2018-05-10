#ifndef INC_MAIN_H
#define INC_MAIN_H

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "sevseg.h"
#include "lcd.h"
#include "keymatrix.h"
#include "itoa.h"
#include "uart.h"
#include "rs485.h"
#include "rtc.h"
#include "adc.h"
#include "lm35.h"

#define BIT(x) (1 << (x))
#define SETBITS(x, y) ((x) |= (y))
#define SETBIT(x, y) SETBITS((x), (BIT((y))))
#define CLEARBITS(x, y) ((x) &= (~(y)))
#define CLEARBIT(x, y) CLEARBITS((x), (BIT((y))))

#endif