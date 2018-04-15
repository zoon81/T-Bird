#ifndef INC_UART_H
#define INC_UART_H
#include "main.h"

// This is only for 16Mhz
typedef enum {
    BAUD_9600   = 103,
    BAUD_19200  = 51,
    BAUD_38400  = 25,
    BAUD_76800  = 12 
} uart_baud_t;

void UART0_Init(uart_baud_t baudrate);
void UART0_sendbyte(uint8_t data);
uint8_t UART0_receivebyte();
void UART0_sendString(char *str);
void UART0_setReceiverCallback( void (*UartCallback)(uint8_t) );
void UART1_Init(uart_baud_t baudrate);
void UART1_sendbyte(uint8_t data);
uint8_t UART1_receivebyte();
void UART1_sendString(char *str);
void UART1_setReceiverCallback( void (*UartCallback)(uint8_t) );
#endif