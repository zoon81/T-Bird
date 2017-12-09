/*
 * lcd.c
 *
 *  Created on: 2017. szept. 25.
 *      Author: ban1zoltan803
 */
#include "lcd.h"

void LCD_Command(uint8_t cmd) {
	LCD_SET_RS_COMMAND
	LCD_sendbyte(cmd);
	_delay_ms(2);
}

void LCD_Data(uint8_t data) {
	LCD_SET_RS_DATA
	LCD_sendbyte(data);
	_delay_ms(2);
}

void LCD_Init(void) 
{
    LCD_data_Dir |= 0xF0; 
    LCD_control_Dir |= 0x0E;
	_delay_ms(20); // LCD Power ON delay  >15ms

	LCD_Command(0x02); // send for 4 bit initialization of LCD
	LCD_Command(0x28); // 2 line, 5*7 matrix in 4-bit mode
	LCD_Command(0x0c); // Display on cursor off
	LCD_Command(0x06); // Increment cursor (shift cursor to right)
	LCD_Command(0x01); // Clear display screen
}

void LCD_String(char *str) /* Send string to LCD function */
{
	while(*str) /* Send each char of string till the NULL */
	{
		LCD_Data(*str++);
	}
}

void LCD_String_xy(uint8_t row, uint8_t pos, char *str) /* Send string to LCD with xy position */
{
	/*	
		Line 1 starts at offset 0×00 0x80 = | 7bit must be 1 for DDRAM address set command | 6bit-4bit all zero because of the line1 address map | 3bit-0bit cursor offset |
		Line 2 starts at offset 0×40
		Line 3 starts at offset 0×10
		Line 4 starts at offset 0×50
	*/
	if (row == 0 && pos < 16)
		LCD_Command((pos & 0x0F) | 0x80); 
	else if (row == 1 && pos < 16)
		LCD_Command((pos & 0x0F) | 0xC0); 
	else if (row == 2 && pos < 16)
		LCD_Command((pos & 0x0F) | 0x90); 
	else if (row == 3 && pos < 16)
		LCD_Command((pos & 0x0F) | 0xD0); 
	LCD_String(str); 
}
// Clear the content of the given $row from specified position $from and clear $len number of position. After that, It will rewind the pointer to the original place
void LCD_Clear_xy(uint8_t row, uint8_t from, uint8_t len){
	LCD_String_xy(row, from, 0);
	if( (from + len) < 16 ){
		while(len--){
			LCD_Data(0x20);
		}
	}
	LCD_String_xy(row, from-1, 0);
}

void LCD_Clear() {
	LCD_Command(0x01); /* Clear display */
	_delay_ms(2);
	LCD_Command(0x80); /* Cursor at home position */
}
void LCD_sendbyte(uint8_t data) {
	LCD_data_Port = (LCD_data_Port & 0x0F) | (data & 0xf0); //upper 4bit, Clearing the upper 4bit of LCD_data_Port and inserting here the data upper 4bit
	LCD_CLOCK_OUT
	LCD_data_Port = (LCD_data_Port & 0x0F) | (data << 4); //lower 4bit, Clearing the upper 4bit of LCD_data_Port and inserting here the data lower 4 bit by shifting it with 4
	LCD_CLOCK_OUT
}
//set a custom char at cgrampos location (gcram max is 8)
void LCD_setCustomChar(uint8_t cgrampos, uint8_t *pixmap){
	LCD_Command(0x40 + (cgrampos * 8));
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	LCD_Data(*pixmap++);
	//set right RAM location for future work
	LCD_Command(0x80);
}
