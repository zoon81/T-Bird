#include "uart.h"

void (*uart_rx_callback_0)(uint8_t);
void (*uart_rx_callback_1)(uint8_t);

void UART0_Init(uart_baud_t baudrate){
    
    UBRR0L = baudrate;
    UBRR0H = baudrate >> 8;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8bit packet size

}

void UART1_Init(uart_baud_t baudrate){
    
    UBRR1L = baudrate;
    UBRR1H = baudrate >> 8;

    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); //8bit packet size

}

void UART0_sendbyte(uint8_t data){
    UDR0 = data;
    while( !(UCSR0A & (1 << UDRE0))); //wait until transmission complete
}

void UART1_sendbyte(uint8_t data){
    UDR1 = data;
    while( !(UCSR1A & (1 << UDRE1))); //wait until transmission complete
}

uint8_t UART0_receivebyte(){
    while( !(UCSR0A & (1 << RXC0))); //wait until receive complete
    return UDR0;
}

uint8_t UART1_receivebyte(){
    while( !(UCSR1A & (1 << RXC1))); //wait until receive complete
    return UDR1;
}

void UART0_sendString(char *str){
    while( *str){
        UART0_sendbyte(*str++);
    }
}

void UART1_sendString(char *str){
    while( *str){
        UART1_sendbyte(*str++);
    }
}

void UART0_setReceiverCallback( void (*UartCallback)(uint8_t) ) {
    //Enable ISR on receive
    UCSR0B |= BIT(RXCIE0);
    sei();
    uart_rx_callback_0 = UartCallback;

}

void UART1_setReceiverCallback( void (*UartCallback)(uint8_t) ) {
    //Enable ISR on receive
    UCSR1B |= BIT(RXCIE1);
    sei();
    uart_rx_callback_1 = UartCallback;

}

ISR(USART0_RX_vect){
    uart_rx_callback_0(UDR0);
}

ISR(USART1_RX_vect){
    uart_rx_callback_1(UDR1);
}