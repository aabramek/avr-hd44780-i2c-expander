#include "lcd.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

int main()
{
	TWBR = 0x0B;
	
	struct LCD lcd;
	LCD_Create(&lcd, 0x27);
	LCD_Init(&lcd);
	LCD_Print(&lcd, "Ala ma kota", 11);
	while (1)
	{
		
	}
}
