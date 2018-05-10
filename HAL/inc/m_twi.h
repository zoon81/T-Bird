#ifndef INC_TWI_H
#define INC_TWI_H
#include "main.h"
#include <compat/twi.h>

#define I2C_READ    1
#define I2C_WRITE   0
#define _I2CMASTER_H   1
#define SCL_CLOCK  50000L

void i2c_init(void);
uint8_t i2c_start(unsigned char address);
void i2c_start_wait(unsigned char address);
unsigned char i2c_rep_start(unsigned char address);
void i2c_stop(void);
unsigned char i2c_write(unsigned char data);
unsigned char i2c_readAck(void);
unsigned char i2c_readNak(void);

#endif