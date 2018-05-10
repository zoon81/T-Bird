#ifndef INC_RS485_H
#define INC_RS485_H
#include "uart.h"

// #define RS485_DRIVE_PORT PORTC
// #define RS485_RECEIVE_PORT PORTE
// #define RS485_DRIVE_PIN PC7
// #define RS485_RECEIVE_PIN PE2

#define RS485_DRIVE_DD DDRB
#define RS485_RECEIVE_DD DDRB
#define RS485_DRIVE_PORT PORTB
#define RS485_RECEIVE_PORT PORTB
#define RS485_DRIVE_PIN PB1
#define RS485_RECEIVE_PIN PB3

void rs485_Init(uart_baud_t );
void rs485_setReceiveCallBack(void (*callback) (uint8_t data) );
void rs485_setReceiverMode();
void rs485_setTransmitMode();

#endif