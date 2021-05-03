#ifndef PAPIEZ
#define PAPIEZ

struct LCD
{
	unsigned char backlight;
	// [LOW, LOW, LOW, LOW, backlight on/off, LOW, LOW, LOW]
	// 4th bit is used to remember wheter the lcd's backlight is turned on or off
	unsigned char address_rw;
	// PCF8574 address shifted one bit to the left plus read/write bit
	// Only field to be modified by user
	unsigned int stringLength;
	// Length of the current string being displayed on the screen
};

// Enables backlight however changes are made after sending instruction or letter to the display
void LCD_BacklightOn(struct LCD* lcd);

// Likewise
void LCD_BacklightOff(struct LCD* lcd);

// Helper function, sends byte of data via I2C
void LCD_SendByte(unsigned char byte, unsigned char address);

// Sends instruction to the display
void LCD_SendInstruction(struct LCD* lcd, unsigned char instruction);

// Sends letter to the display
void LCD_SendLetter(struct LCD* lcd, char letter);

// Initializes LCD structure
void LCD_Create(struct LCD* lcd, unsigned char address);

// Initializes LCD screen
void LCD_Init(struct LCD* lcd);

// Sends string to the display
unsigned int LCD_Print(struct LCD* lcd, const char* string, unsigned int length);

#endif