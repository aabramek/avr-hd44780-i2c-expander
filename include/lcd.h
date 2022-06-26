#ifndef LCD_H
#define LCD_H

#include <stdint.h>

struct LCD
{
	uint8_t backlight;
	// [0, 0, 0, 0, backlight on/off, 0, 0, 0]
	// 4th bit is used to remember wheter the lcd's backlight is turned on or off
	uint8_t address;
	// PCF8574 address shifted one bit to the left plus read/write bit
};

// Helper function, sends byte of data via I2C
void I2C_SendByte(uint8_t byte, uint8_t address);

// Sends instruction to the display
void LCD_SendInstruction(struct LCD* lcd, uint8_t instruction);

// Sends letter to the display
void LCD_SendData(struct LCD* lcd, char letter);

// Initializes LCD structure
void LCD_Create(struct LCD* lcd, uint8_t address, uint8_t backlight);

// Initializes LCD screen
void LCD_Init(struct LCD* lcd, uint8_t display_params);

// Sends string to the display
void LCD_Print(struct LCD* lcd, const char* string, uint8_t length);

// Defines custom character
void LCD_CustomCharacter(struct LCD* lcd, uint8_t code, uint8_t pattern[8]);

// Macros with lcd commands
#define LCD_CMD_CLEAR  (0x01)
 
#define LCD_CMD_RETURN (0x02)
 
#define LCD_CMD_ENTRY_MODE (0x04)
    #define LCD_PARAM_INCREMENT	(0x02)
    #define LCD_PARAM_DECREMENT	(0x00)
    #define LCD_PARAM_SHIFT		(0x01)
    #define LCD_PARAM_NOSHIFT	(0x00)
    
#define LCD_CMD_DISPLAY_ONOFF (0x08)
    #define LCD_PARAM_DISPLAY_ON  (0x04)
    #define LCD_PARAM_DISPLAY_OFF (0x00)
    #define LCD_PARAM_CURSOR_ON   (0x02)
    #define LCD_PARAM_CURSOR_OFF  (0x00)
    #define LCD_PARAM_BLINK_ON    (0x01)
    #define LCD_PARAM_BLINK_OFF   (0x00)
    
#define LCD_CMD_DISPLAY_CURSOR_SHIFT (0x10)
    #define LCD_PARAM_SHIFT_CURSOR	(0x00)
    #define LCD_PARAM_SHIFT_DISPLAY	(0x08)
    #define LCD_PARAM_SHIFT_LEFT	(0x00)
    #define LCD_PARAM_SHIFT_RIGHT	(0x04)
 
#define LCD_CMD_FUNCTION_SET (0x20)
    #define LCD_PARAM_FONT5x8	(0x00)
    #define LCD_PARAM_FONT5x10	(0x04)
    #define LCD_PARAM_ONE_LINE	(0x00)
    #define LCD_PARAM_TWO_LINE	(0x08)
    #define LCD_PARAM_4BIT		(0x00)
    #define LCD_PARAM_8BIT		(0x10)
 
#define LCD_CMD_CGRAM_SET (0x40)
 
#define LCD_CMD_DDRAM_SET (0x80)
 
#define LCD_Clear(lcd)				{ LCD_SendInstruction(lcd, LCD_CMD_CLEAR); }
#define LCD_ReturnHome(lcd)			{ LCD_SendInstruction(lcd, LCD_CMD_RETURN); }

#define LCD_ShiftDisplayRight(lcd)	{ LCD_SendInstruction(lcd, LCD_CMD_DISPLAY_CURSOR_SHIFT |\
															   LCD_PARAM_SHIFT_DISPLAY |\
															   LCD_PARAM_SHIFT_RIGHT); }
														
#define LCD_ShiftDisplayLeft(lcd)	{ LCD_SendInstruction(lcd, LCD_CMD_DISPLAY_CURSOR_SHIFT |\
															   LCD_PARAM_SHIFT_DISPLAY |\
															   LCD_PARAM_SHIFT_LEFT); }
														
#define LCD_SetPosition(lcd, X, Y) 	{ LCD_SendInstruction(lcd, 0x80 | (X + Y * 0x40)); }
#define LCD_BacklightOn(lcd) 		{ (lcd)->backlight = 0x08; I2C_SendByte(0x08, (lcd)->address); }
#define LCD_BacklightOff(lcd) 		{ (lcd)->backlight = 0x00; I2C_SendByte(0x00, (lcd)->address); }

#endif