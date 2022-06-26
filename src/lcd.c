#include "lcd.h"

#include <avr/io.h>
#include <util/delay.h>

void I2C_SendByte(uint8_t byte, uint8_t address)
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

void LCD_SendInstruction(struct LCD* lcd, uint8_t instruction)
{
	// shift bytes so they will appear on P7 - P4 outputs of PCF8574 instead of P3 - P0
	// RegisterSelect and Enable bits stay LOW, Backlight is set/unset according to settings
	uint8_t instructionHigh = (instruction & 0xF0) | lcd->backlight;
	uint8_t instructionLow = ((instruction & 0x0F) << 4) | lcd->backlight;
	
	// First send high part of instruction
	I2C_SendByte(instructionHigh | 0x04, lcd->address);
	I2C_SendByte(instructionHigh, lcd->address);
	
	// Then send lower one
	I2C_SendByte(instructionLow | 0x04, lcd->address);
	I2C_SendByte(instructionLow, lcd->address);
}

void LCD_SendLetter(struct LCD* lcd, char letter)
{
	// First the 4 most significant bits are sent, then 4 least significant ones
	// RegisterSelect is HIGH, Enable bit stays LOW, Backlight is set/unset according to settings
	char letterHigh = (letter & 0xF0) | lcd->backlight | 0x1;
	char letterLow = ((letter & 0x0F) << 4) | lcd->backlight | 0x1;
	
	I2C_SendByte(letterHigh | 0x04, lcd->address);
	I2C_SendByte(letterHigh, lcd->address);
	
	// Then send lower one
	I2C_SendByte(letterLow | 0x04, lcd->address);
	I2C_SendByte(letterLow, lcd->address);
}

void LCD_Create(struct LCD* lcd, uint8_t address, uint8_t backlight)
{
	lcd->address = address << 1;
	lcd->backlight = backlight << 3;
}

void LCD_Init(struct LCD* lcd, uint8_t display_params)
{	
	// Start initialization sequence
	I2C_SendByte(0x34, lcd->address);
	I2C_SendByte(0x30, lcd->address);
	
	_delay_ms(4); // 4.1ms required
	
	I2C_SendByte(0x34, lcd->address);
	I2C_SendByte(0x30, lcd->address);
	
	I2C_SendByte(0x34, lcd->address);
	I2C_SendByte(0x30, lcd->address);
	
	// ====================
	
	I2C_SendByte(0x24, lcd->address);
	I2C_SendByte(0x20, lcd->address);
	
	// Function Set
	LCD_SendInstruction(lcd, LCD_CMD_FUNCTION_SET | LCD_PARAM_FONT5x8 | LCD_PARAM_TWO_LINE | LCD_PARAM_4BIT);
	
	// Display on/off
	LCD_SendInstruction(lcd, LCD_CMD_DISPLAY_ONOFF | display_params);
	
	// Display clear
	LCD_SendInstruction(lcd, LCD_CMD_CLEAR);
}

void LCD_Print(struct LCD* lcd, const char* string, uint8_t length)
{
	for (unsigned int i = 0; i < length; ++i)
	{
		LCD_SendLetter(lcd, string[i]);
	}
}

void LCD_CustomCharacter(struct LCD* lcd, uint8_t code, uint8_t pattern[8])
{
	LCD_SendInstruction(lcd, LCD_CMD_CGRAM_SET | code << 3);
	for (uint8_t i = 0; i < 8; ++i)
	{
		LCD_SendLetter(lcd, pattern[i]);
	}
}
