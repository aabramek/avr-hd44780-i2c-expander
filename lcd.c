#include "lcd.h"

#include <avr/io.h>
#include <util/delay.h>

void LCD_BacklightOn(struct LCD* lcd)
{
	lcd->backlight = 0x8;
}

void LCD_BacklightOff(struct LCD* lcd)
{
	lcd->backlight = 0;
}

void LCD_SendByte(unsigned char byte, unsigned char address)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // start signal
	while(!(TWCR & (1 << TWINT)));

	TWDR = address; // pcf8574 address
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));

	TWDR = byte; // data
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));

	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // stop signal
}

void LCD_SendInstruction(struct LCD* lcd, unsigned char instruction)
{
	// shift bytes so they will appear on P7 - P4 outputs of PCF8574 instead of P3 - P0
	// RegisterSelect and Enable bits stay LOW, Backlight is set/unset according to settings
	unsigned char instructionHigh = (instruction & 0xF0) | lcd->backlight;
	unsigned char instructionLow = ((instruction & 0x0F) << 4) | lcd->backlight;
	
	// First send high part of instruction
	LCD_SendByte(instructionHigh, lcd->address_rw);
	LCD_SendByte(instructionHigh | 0x04, lcd->address_rw);
	LCD_SendByte(instructionHigh, lcd->address_rw); // possible to be removed in future releases
	
	// Then send lower one
	LCD_SendByte(instructionLow, lcd->address_rw);
	LCD_SendByte(instructionLow | 0x04, lcd->address_rw);
	LCD_SendByte(instructionLow, lcd->address_rw); // possible to be removed in future releases
}

void LCD_SendLetter(struct LCD* lcd, char letter)
{
	// First the 4 most significant bits are sent, then 4 least significant ones
	// RegisterSelect is HIGH, Enable bit stays LOW, Backlight is set/unset according to settings
	char letterHigh = (letter & 0xF0) | lcd->backlight | 0x1;
	char letterLow = ((letter & 0x0F) << 4) | lcd->backlight | 0x1;
	
	LCD_SendByte(letterHigh, lcd->address_rw);
	LCD_SendByte(letterHigh | 0x04, lcd->address_rw);
	LCD_SendByte(letterHigh, lcd->address_rw); // possible to be removed in future releases
	
	// Then send lower one
	LCD_SendByte(letterLow, lcd->address_rw);
	LCD_SendByte(letterLow | 0x04, lcd->address_rw);
	LCD_SendByte(letterLow, lcd->address_rw); // possible to be removed in future releases
}

void LCD_Create(struct LCD* lcd, unsigned char address)
{
	lcd->address_rw = address << 1;
	lcd->backlight = 0x8;
	lcd->stringLength = 0;
}

void LCD_Init(struct LCD* lcd)
{
	// Clear all PCF8574 outputs
	LCD_SendInstruction(lcd, 0x00); 
	
	// Start initialization sequence
	LCD_SendByte(0x30, lcd->address_rw);
	LCD_SendByte(0x34, lcd->address_rw);
	LCD_SendByte(0x30, lcd->address_rw);
	
	_delay_ms(5); // 4.1ms required
	
	LCD_SendByte(0x30, lcd->address_rw);
	LCD_SendByte(0x34, lcd->address_rw);
	LCD_SendByte(0x30, lcd->address_rw);
	
	_delay_us(150); // 100us required
	
	LCD_SendByte(0x30, lcd->address_rw);
	LCD_SendByte(0x34, lcd->address_rw);
	LCD_SendByte(0x30, lcd->address_rw);
	_delay_us(60); // 37us required
	
	// ====================
	
	LCD_SendByte(0x20, lcd->address_rw);
	LCD_SendByte(0x24, lcd->address_rw);
	LCD_SendByte(0x20, lcd->address_rw);
	_delay_us(60); // 37us required
	
	// Function Set
	LCD_SendInstruction(lcd, 0x28);
	_delay_us(60); // 37us required
	
	// Display on, Cursor on, Blinking on
	LCD_SendInstruction(lcd, 0x0F);
	_delay_us(60); // 37us required
	
	// Display clear + backlight
	LCD_SendInstruction(lcd, 0x01);
	_delay_us(60); // 37us required
}

unsigned int LCD_Print(struct LCD* lcd, const char* string, unsigned int length)
{
	unsigned int charsWritten = 0;
	for (unsigned int i = 0; i < length; ++i)
	{
		LCD_SendLetter(lcd, string[i]);
	}
	return charsWritten;
}
