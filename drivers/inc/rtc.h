#ifndef INC_RTC_H
#define INC_RTC_H

#include <avr/io.h>
#include "m_twi.h"

#define RTC_ADDR_W 0xA2
#define RTC_ADDR_R 0xA3

#define CONTROL_STATUS_1    0x00
#define CONTROL_STATUS_2    0x01
#define VL_SECONDS          0x02
#define MINUTES             0x03
#define HOURS               0x04
#define DAYS                0x05
#define WEEKDAYS            0x06
#define CENTURY_MONTHS      0x07
#define YEARS               0x08

typedef struct{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
    uint8_t month;
    uint16_t year;
} DateTime;

void rtc_Init();
uint8_t rtc_setDateTime(DateTime *dateTime);
uint8_t rtc_getDateTime(DateTime *dateTime);

#endif