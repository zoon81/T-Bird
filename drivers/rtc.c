#include "rtc.h"
#define BinToBCD(bin) ((((bin) / 10) << 4) + ((bin) % 10))

void rtc_Init(){
    i2c_init();
    i2c_start(RTC_ADDR_W);
    i2c_write(CONTROL_STATUS_1);
	i2c_write(0x00);
	i2c_write(0x00);
	i2c_stop();
    
}

uint8_t rtc_setDateTime(DateTime *dateTime){
	if (dateTime->second >= 60 || dateTime->minute >= 60 || dateTime->hour >= 24 || dateTime->day > 32 || dateTime->weekday > 6 || dateTime->month > 12 || dateTime->year < 1900 || dateTime->year >= 2100)
	{
		return 1;
	}

	i2c_start(RTC_ADDR_W);
	i2c_write(VL_SECONDS);
	i2c_write(BinToBCD(dateTime->second) & 0x7F);
	i2c_write(BinToBCD(dateTime->minute) & 0x7F);
	i2c_write(BinToBCD(dateTime->hour) & 0x3F);
	i2c_write(BinToBCD(dateTime->day) & 0x3F);
	i2c_write(BinToBCD(dateTime->weekday) & 0x07);
 	

	if (dateTime->year >= 2000){
		i2c_write( (BinToBCD(dateTime->month) & 0x1F) | 0x80);
		i2c_write(BinToBCD(dateTime->year - 2000));
	}
	else{
		i2c_write(BinToBCD(dateTime->month) & 0x1F);
		i2c_write(BinToBCD(dateTime->year - 1900));
	}

	i2c_stop();
	return 0;
}

uint8_t rtc_getDateTime(DateTime *dateTime){
	uint8_t buffer[7];

	i2c_start(RTC_ADDR_W);
	i2c_write(VL_SECONDS); 
	i2c_rep_start(RTC_ADDR_R);
	buffer[0] = i2c_readAck();
	buffer[1] = i2c_readAck();
	buffer[2] = i2c_readAck();
	buffer[3] = i2c_readAck();
	buffer[4] = i2c_readAck();
	buffer[5] = i2c_readAck();
	buffer[6] = i2c_readNak();
	i2c_stop();

	dateTime->second = (((buffer[0] >> 4) & 0x07) * 10) + (buffer[0] & 0x0F);
	dateTime->minute = (((buffer[1] >> 4) & 0x07) * 10) + (buffer[1] & 0x0F);
	dateTime->hour = (((buffer[2] >> 4) & 0x03) * 10) + (buffer[2] & 0x0F);
	dateTime->day = (((buffer[3] >> 4) & 0x03) * 10) + (buffer[3] & 0x0F);
	dateTime->weekday = (buffer[4] & 0x07);
	dateTime->month = ((buffer[5] >> 4) & 0x01) * 10 + (buffer[5] & 0x0F);
	dateTime->year = 1900 + ((buffer[6] >> 4) & 0x0F) * 10 + (buffer[6] & 0x0F);

	if (buffer[5] &  0x80)
	{
		dateTime->year += 100;
	}

	if (buffer[0] & 0x80) //Clock integrity not guaranted
	{
		return 1;
	}

	
	return 0;
}