#include "main.h"
#include <stdlib.h>

void uart0ReceiveCallBack(uint8_t);
void rs485ReceiveCallBack(uint8_t);


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
uint8_t fifo_write(char *source, uint8_t len, struct fifo_t *fifo){+
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
    // rtc_Init();
    // DateTime date_time = {0, 37, 7, 30, 1, 4, 2018};
    // //rtc_setDateTime(&date_time);
    // rtc_getDateTime(&date_time);
    // struct fifo_t uartFifo;
    // char buffer[16];
    // fifo_init(buffer, 16, &uartFifo);
    // fifo_write("fifotesttext", 13, &uartFifo);
    // char data[16];
    // fifo_read(data, 13, &uartFifo);
    // fifo_write("fifotesttext", 13, &uartFifo);
    // fifo_write("f", 2, &uartFifo);
    // fifo_read(data, 15, &uartFifo);
    // adc_init();
    
    UART0_Init(BAUD_9600);
    UART0_setReceiverCallback(uart0ReceiveCallBack);
    UART0_sendString("\n\rHey\n\r");
    
    rs485_Init(BAUD_9600);
    rs485_setReceiveCallBack(rs485ReceiveCallBack);
    while(1){
        
    }
    
}

void uart0ReceiveCallBack(uint8_t data){
    UART0_sendbyte(data);
    rs485_setTransmitMode();
    UART1_sendbyte_blocking(data);
    rs485_setReceiverMode();
}

void rs485ReceiveCallBack(uint8_t data){
    UART0_sendbyte(data);
    
}