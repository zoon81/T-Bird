#include "main.h"
#include <stdlib.h>

void setleds(uint8_t);
void uartReceiveCallBack(uint8_t);

void main()
{
    LCD_Init();
    LCD_String_xy(0, 0, "UART CALC");
    LCD_String_xy(1, 0, "Hit #for + - * /");
    UART0_Init(BAUD_9600);
    UART0_sendString("\n\n\n\rUART Calculator\n\rUse the keypad on the T-bird to enter the function\n\r");
    UART0_setReceiverCallback(uartReceiveCallBack);
    keyboardmatrix_init();
    uint16_t num1 = 0;
    uint16_t num2 = 0;
    char buffer[7] = {0};
    char *buffer_ptr = buffer;
    char op_symb[] = {'/', '+', '-', '*'};
    uint8_t op_symb_index = 0;
    LCD_String_xy(2, 0, "");
    do
    {
        *buffer_ptr = keyboardmatrix_getchar();

        if (*buffer_ptr == '#')
        {
            *buffer_ptr = 0;
            num1 = atoi_(buffer);
            buffer_ptr = buffer;
            UART0_sendbyte(' ');
            UART0_sendString("\x1B\x5B\x43");
            do
            {
                LCD_String_xy(2, 6, "");
                UART0_sendString("\x1B\x5B\x44");
                ++op_symb_index;
                if (op_symb_index > 3)
                {
                    op_symb_index = 0;
                }
                LCD_Data(op_symb[op_symb_index]);
                UART0_sendbyte(op_symb[op_symb_index]);
                LCD_String("  ");
                *buffer_ptr = keyboardmatrix_getchar();
            } while (*buffer_ptr == '#');
            UART0_sendbyte(' ');
        }
        if ((*buffer_ptr >= 48) && (*buffer_ptr < 58))
        {
            LCD_Data(*buffer_ptr);
            UART0_sendbyte(*buffer_ptr);
            buffer_ptr++;
        }
    } while (*buffer_ptr != '*');
    *buffer_ptr = 0;
    num2 = atoi_(buffer);
    int16_t result = 0;
    switch (op_symb[op_symb_index])
    {
    case '+':
        result = num1 + num2;
        break;
    case '-':
        result = num1 - num2;
        break;
    case '*':
        result = num1 * num2;
        break;
    case '/':
        result = num1 / num2;
        break;
    default:
        break;
    }
    uint16_to_str(buffer, num1);
    LCD_String_xy(3, 0, buffer);
    LCD_Data(op_symb[op_symb_index]);
    uint16_to_str(buffer, num2);
    LCD_String(buffer);
    LCD_String("=");
    UART0_sendString(" = ");
    int16_to_str(buffer, result);
    LCD_String(buffer);
    UART0_sendString(buffer);
    while (1)
    {
    }
}

void setleds(uint8_t value)
{
    PORTD = (value & 0xF0);
    PORTB = ((value & 0x0F) << 4);
}

void uartReceiveCallBack(uint8_t data)
{

    UART0_sendbyte(data);
}