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

#define BIT(x) (1 << (x))

struct portstate_s{
    uint8_t port_a;
    uint8_t port_c;
    uint8_t port_e;
    uint8_t port_f;
    uint8_t period;
};

#define RW_RED_1 PF1
#define RW_RED_2 PF2
#define RW_WHITE PF3
#define RW_SENSE_LEFT PE4
#define RW_SENSE_RIGHT PE5


#endif