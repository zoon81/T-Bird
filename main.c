#include "main.h"
#include <stdlib.h>

void setleds(uint8_t);
void uart0ReceiveCallBack(uint8_t);
void uart1ReceiveCallBack(uint8_t);
void rs485_setReceiverMode();
void rs485_setTransmitMode();

struct fifo_t {
    uint8_t *write_p;
    uint8_t *read_p;
    uint8_t *base;
    uint8_t size;
};

void fifo_init(char *buffer, uint8_t buffer_len, struct fifo_t *fifo ){
    fifo->write_p   = &buffer[0];
    fifo->read_p    = &buffer[0];
    fifo->base      = &buffer[0]; 
    fifo->size = buffer_len;
}
uint8_t fifo_write(char *source, uint8_t len, struct fifo_t *fifo){
    uint8_t index = 0;
    while(index < len){
        if(fifo->write_p - fifo->base >= fifo->size)
            fifo->write_p = fifo->base;
        if(fifo->write_p == fifo->read_p - 1)
            return index;
        *fifo->write_p++ = *source++;
        index++;
    }
}
uint8_t fifo_read(char *dest, uint8_t len, struct fifo_t *fifo){
    uint8_t index = 0;
    while(index < len){
        if(fifo->read_p - fifo->base >= fifo->size)
            fifo->read_p = fifo->base;
        if(fifo->write_p - 1 == fifo->read_p)
            return index;
        *dest++ = *fifo->read_p++;
        index++;
    }
}


void main()
{
    struct fifo_t uartFifo;
    char buffer[16];
    fifo_init(buffer, 16, &uartFifo);
    fifo_write("fifotesttext", 12, &uartFifo);
    char data[16];
    fifo_read(data, 12, &uartFifo);
    fifo_write("fifotesttext", 12, &uartFifo);
    fifo_write("f", 2, &uartFifo);
    fifo_read(data, 14, &uartFifo);
    DDRB = 0xF0;
    DDRC = BIT(PC7);
    DDRE = BIT(PE2);
    uint16_t adresult, celsius;
    UART0_Init(51);
    UART1_Init(51);
    UART0_sendString("\n\rHey\n\r");
    UART0_setReceiverCallback(uart0ReceiveCallBack);
    //rs485_setTransmitMode();
    //UART1_sendbyte(0xff);
    rs485_setReceiverMode();
    UART1_setReceiverCallback(uart1ReceiveCallBack);

    while (1){
    }
}

void uart0ReceiveCallBack(uint8_t data){
    UART0_sendbyte(data);
}

void uart1ReceiveCallBack(uint8_t data){
    UART0_sendbyte(data);
}