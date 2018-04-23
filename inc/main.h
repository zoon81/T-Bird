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
#define SW_K0 PG0
#define SW_K1 PG1
#define SW_K2 PG2
#define SW_K3 PG3
#define SW_K4 PG4
struct pindesc_s{
    volatile uint8_t *port;
    uint8_t pin;
};

struct blink_state_s{
    uint8_t blinking_pins;
    struct pindesc_s pindesc[5];
};

struct portstate_s{
    uint8_t port_a;
    uint8_t port_c;
    uint8_t port_e;
    uint8_t port_f;
    uint8_t period;
    struct blink_state_s blinkstate;
};

#define BLINK_NONE 8
#define RW_RED_1 PF1
#define RW_RED_2 PF2
#define RW_WHITE PF3
#define RW_SENSE_LEFT PE4
#define RW_SENSE_RIGHT PE5
#define MODE_TRAIN_GONE 0
#define MODE_TRAIN_FROM_LEFT 1
#define MODE_TRAIN_FROM_RIGHT 2
#define MODE_SERVICE_REQUEST 5
#define MODE_SERVICE 6
#define MODE_TRAIN_RED 7
#define TC_RAILWAY_CLOSED_STATE 8


#endif