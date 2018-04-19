#include "main.h"
#include <stdlib.h>

void setleds(uint8_t);
void uart0ReceiveCallBack(uint8_t);
void uart1ReceiveCallBack(uint8_t);
void rs485_setReceiverMode();
void rs485_setTransmitMode();

#define FIFO_SIZE 64
char fifo_buffer[FIFO_SIZE];
char *buffer_ptr_w = fifo_buffer;
uint8_t buffer_w_len = 0;
uint8_t buffer_w_pos = 1;
char *buffer_ptr_r = fifo_buffer;
uint8_t buffer_r_pos = 0;

void main()
{
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


uint8_t fifo_readOut(char *buffer, uint8_t len){
    if(buffer_w_len < len){                             //Check fifo len
        len = buffer_w_len;
    }
    uint8_t i;                                          //Copy from FiFo to buffer
    for(i = 0; i < len; i++){
        if(buffer_r_pos > FIFO_SIZE)
            buffer_r_pos = 0;
        
        buffer[i] = buffer_ptr_r[buffer_r_pos++];       
    }
    buffer_ptr_r = &buffer_ptr_r[buffer_r_pos];         //Set fifo read pointer
    return len;
}

uint8_t fifo_writeIn(char *buffer, uint8_t len){
    if(buffer_r_pos == buffer_w_pos){                   // Out of space
        return 0;
    }
    if(len > FIFO_SIZE){                                //Size limit
        len = FIFO_SIZE;
    }
    uint8_t i;
    for(i < 0 ; i < len; i++){
        if(buffer_w_pos > FIFO_SIZE)
            buffer_w_pos = 0;
        buffer_ptr_w[buffer_w_pos++] = buffer[i];
    }
    buffer_ptr_w = buffer_ptr_w[buffer_w_pos];       

}