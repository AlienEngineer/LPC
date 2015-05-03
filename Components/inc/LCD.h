/*
 * LCD.h
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#ifndef LCD_H_
#define LCD_H_

#include <common.h>
#include <PIN.h>
#include <SPI.h>

typedef struct {
	uint32_t position;
	uint32_t size;
} Coord;

typedef struct {
	uint32_t x;
	uint32_t y;
} Point;

// Reserve some space for coordinates

#define LCD_SIZE 		(132)



#define WHITE			0xFF
#define BLACK			0x00

#define LINE_HEIGHT		(10)
#define COLUMN_WIDTH	(8)
#define WIDTH			(132)
#define HEIGHT			(132)
#define START_AT		(10)

class LCD {
public:
	LCD(PIN * csPin, PIN * resetPin);
	void ClearArea(Coord * first, Coord * second);
	void SetCursor(uint32_t x, uint32_t y);
	void Write(char * str);
	void ClearScreen();

private:
	void Command(uint32_t command);
	void Data(uint32_t data);
	void PutChar(char c, unsigned char * pFont);
	void LineBreak();

	PIN * csPin;
	SPI spi;
	Point current_position;
};

#endif /* LCD_H_ */
