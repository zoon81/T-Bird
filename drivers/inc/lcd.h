#include "main.h"


#ifndef INC_M_LCD_H_
#define INC_M_LCD_H_

#define LCD_data_Dir  DDRE			/* Define LCD data port direction */
#define LCD_data_Port PORTE			/* Define LCD data port */
#define LCD_control_Dir  DDRF			/* Define LCD data port direction */
#define LCD_control_Port PORTF			/* Define LCD data port */

#define RS PF1				// Define Register Select pin
#define RW PF2              // Define R/W pin
#define EN PF3 				// Define Enable signal pin

#define LCD_CLOCK_OUT 	LCD_control_Port |= (1<<EN);asm("nop");asm("nop");LCD_control_Port &= ~ (1<<EN);
#define LCD_SET_RS_DATA		LCD_control_Port |=  (1<<RS);
#define LCD_SET_RS_COMMAND	LCD_control_Port &= ~(1<<RS);

void LCD_Init(void);
void LCD_Command(uint8_t cmnd);
void LCD_Data(uint8_t data);
void LCD_String(char *str);
void LCD_String_xy(uint8_t row, uint8_t pos, char *str);
void LCD_Clear();
void LCD_sendbyte(uint8_t data);
void LCD_setCustomChar(uint8_t cgrampos, uint8_t *pixmap);

#endif /* INC_M_LCD_H_ */
