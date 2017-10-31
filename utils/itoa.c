#include "itoa.h"
void uint8_to_str(char *buffer, uint8_t number){
    uint8_t data;
	if(number > 99){
		data = number / 100;
		number %=  100;
		*buffer++ = data + 48;
		data = number / 10;
		number %= 10;
		*buffer++ = data + 48;
        *buffer++ = number + 48;
        *buffer = 0;                //string termination
	}else if(number > 9){
		data = number / 10;
		number %= 10;
		*buffer++ = data + 48;
        *buffer++ = number + 48;
        *buffer = 0;                //string termination
	} else if(number >= 0){
        *buffer++ = number + 48;
        *buffer = 0;                //string termination
	}
}