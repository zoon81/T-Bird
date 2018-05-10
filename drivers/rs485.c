#include "rs485.h"

void rs485_Init(uart_baud_t baudrate){
    UART1_Init(baudrate);
    RS485_DRIVE_DD |= BIT(RS485_DRIVE_PIN);
    RS485_RECEIVE_DD |= BIT(RS485_RECEIVE_PIN);
    rs485_setReceiverMode();
}

void rs485_setReceiveCallBack(void (*callback) (uint8_t data) ){
    UART1_setReceiverCallback(callback);
}

void rs485_setReceiverMode(){
    RS485_DRIVE_PORT &= ~BIT(RS485_DRIVE_PIN);
    RS485_RECEIVE_PORT &= ~BIT(RS485_RECEIVE_PIN); 
}

void rs485_setTransmitMode(){
    RS485_DRIVE_PORT |= BIT(RS485_DRIVE_PIN);
    RS485_RECEIVE_PORT |= BIT(RS485_RECEIVE_PIN);
}