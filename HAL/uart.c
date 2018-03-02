#include "uart.h"

void (*uart_rx_callback)(uint8_t);

void UART0_Init(uart_baud_t baudrate){
    
    UBRR0L = baudrate;
    UBRR0H = baudrate >> 8;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8bit packet size

}

void UART0_sendbyte(uint8_t data){
    UDR0 = data;
    while( !(UCSR0A & (1 << UDRE0))); //wait until transmission complete
}

uint8_t UART0_receivebyte(){
    while( !(UCSR0A & (1 << RXC0))); //wait until receive complete
    return UDR0;
}

void UART0_sendString(char *str){
    while( *str){
        UART0_sendbyte(*str++);
    }
}

void UART0_setReceiverCallback( void (*UartCallback)(uint8_t) ) {
    //Enable ISR on receive
    UCSR0B |= BIT(RXCIE0);
    sei();
    uart_rx_callback = UartCallback;

}

ISR(USART0_RX_vect){
    uart_rx_callback(UDR0);
}