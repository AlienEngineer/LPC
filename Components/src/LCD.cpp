#ifdef LCD_ENABLE

#include <LCD.h>
#include <SPI.h>
#include <LPCTimer.h>
#include <GPIO.h>
#include <stdio.h>
#include <stdarg.h>

const unsigned char font8x8[97][8] = { 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
		0x00, // columns, rows, num_bytes_per_char
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // space 0x20
		0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00, // !
		0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, // "
		0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, // #
		0x18, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x18, 0x00, // $
		0x00, 0x63, 0x66, 0x0C, 0x18, 0x33, 0x63, 0x00, // %
		0x1C, 0x36, 0x1C, 0x3B, 0x6E, 0x66, 0x3B, 0x00, // &
		0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, // '
		0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00, // (
		0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, // )
		0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, // *
		0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0x00, // +
		0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, // ,
		0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, // -
		0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, // .
		0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00, // / (forward slash)
		0x3E, 0x63, 0x63, 0x6B, 0x63, 0x63, 0x3E, 0x00, // 0 0x30
		0x18, 0x38, 0x58, 0x18, 0x18, 0x18, 0x7E, 0x00, // 1
		0x3C, 0x66, 0x06, 0x1C, 0x30, 0x66, 0x7E, 0x00, // 2
		0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00, // 3
		0x0E, 0x1E, 0x36, 0x66, 0x7F, 0x06, 0x0F, 0x00, // 4
		0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00, // 5
		0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00, // 6
		0x7E, 0x66, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x00, // 7
		0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00, // 8
		0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00, // 9
		0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, // :
		0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, // ;
		0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00, // <
		0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, // =
		0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00, // >
		0x3C, 0x66, 0x06, 0x0C, 0x18, 0x00, 0x18, 0x00, // ?
		0x3E, 0x63, 0x6F, 0x69, 0x6F, 0x60, 0x3E, 0x00, // @ 0x40
		0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x00, // A
		0x7E, 0x33, 0x33, 0x3E, 0x33, 0x33, 0x7E, 0x00, // B
		0x1E, 0x33, 0x60, 0x60, 0x60, 0x33, 0x1E, 0x00, // C
		0x7C, 0x36, 0x33, 0x33, 0x33, 0x36, 0x7C, 0x00, // D
		0x7F, 0x31, 0x34, 0x3C, 0x34, 0x31, 0x7F, 0x00, // E
		0x7F, 0x31, 0x34, 0x3C, 0x34, 0x30, 0x78, 0x00, // F
		0x1E, 0x33, 0x60, 0x60, 0x67, 0x33, 0x1F, 0x00, // G
		0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, // H
		0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // I
		0x0F, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00, // J
		0x73, 0x33, 0x36, 0x3C, 0x36, 0x33, 0x73, 0x00, // K
		0x78, 0x30, 0x30, 0x30, 0x31, 0x33, 0x7F, 0x00, // L
		0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00, // M
		0x63, 0x73, 0x7B, 0x6F, 0x67, 0x63, 0x63, 0x00, // N
		0x3E, 0x63, 0x63, 0x63, 0x63, 0x63, 0x3E, 0x00, // O
		0x7E, 0x33, 0x33, 0x3E, 0x30, 0x30, 0x78, 0x00, // P 0x50
		0x3C, 0x66, 0x66, 0x66, 0x6E, 0x3C, 0x0E, 0x00, // Q
		0x7E, 0x33, 0x33, 0x3E, 0x36, 0x33, 0x73, 0x00, // R
		0x3C, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x3C, 0x00, // S
		0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // T
		0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x00, // U
		0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, // V
		0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00, // W
		0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00, // X
		0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x00, // Y
		0x7F, 0x63, 0x46, 0x0C, 0x19, 0x33, 0x7F, 0x00, // Z
		0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, // [
		0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, // \ (back slash)
		0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, // ]
		0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, // ^
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // _
		0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, // ` 0x60
		0x00, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00, // a
		0x70, 0x30, 0x3E, 0x33, 0x33, 0x33, 0x6E, 0x00, // b
		0x00, 0x00, 0x3C, 0x66, 0x60, 0x66, 0x3C, 0x00, // c
		0x0E, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3B, 0x00, // d
		0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00, // e
		0x1C, 0x36, 0x30, 0x78, 0x30, 0x30, 0x78, 0x00, // f
		0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x7C, // g
		0x70, 0x30, 0x36, 0x3B, 0x33, 0x33, 0x73, 0x00, // h
		0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00, // i
		0x06, 0x00, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, // j
		0x70, 0x30, 0x33, 0x36, 0x3C, 0x36, 0x73, 0x00, // k
		0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // l
		0x00, 0x00, 0x66, 0x7F, 0x7F, 0x6B, 0x63, 0x00, // m
		0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x00, // n
		0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, // o
		0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78, // p 0x70
		0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F, // q
		0x00, 0x00, 0x6E, 0x3B, 0x33, 0x30, 0x78, 0x00, // r
		0x00, 0x00, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x00, // s
		0x08, 0x18, 0x3E, 0x18, 0x18, 0x1A, 0x0C, 0x00, // t
		0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00, // u
		0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, // v
		0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00, // w
		0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00, // x
		0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x7C, // y
		0x00, 0x00, 0x7E, 0x4C, 0x18, 0x32, 0x7E, 0x00, // z
		0x0E, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0E, 0x00, // {
		0x0C, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x00, // |
		0x70, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x70, 0x00, // }
		0x3B, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ~
		0x1C, 0x36, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00  // DEL
		};

#define DISON			0xAF
#define DISOFF			0xAE
#define DISNOR			0xA6
#define DISINV			0xA7
#define COMSCN			0xBB
#define DISCTL			0xCA
#define SLPIN			0x95
#define SLPOUT			0x94
#define PASET			0x75
#define CASET			0x15
#define DATCTL			0xBC
#define RGBSET8			0xCE
#define RAMWR			0x5C
#define RAMRD			0x5D
#define PTLIN			0xA8
#define PTLOUT			0xA9
#define RMWIN			0xE0
#define RMWOUT			0xEE
#define ASCSET			0xAA
#define SCSTART			0xAB
#define OSCON			0xD1
#define OSCOFF			0xD2
#define PWRCTR			0x20
#define VOLCTR			0x81
#define VOLUP			0xD6
#define VOLDOWN			0xD7
#define TMPGRD			0x82
#define EPCTIN			0xCD
#define EPCOUT			0xCC
#define EPMWR			0xFC
#define EPMRD			0xFD
#define EPSRRD1			0x7C
#define EPSRRD2			0x7D
#define NOP				0x25

Coord LCD_Coord1;
Coord LCD_Coord2;
#define Coordinate1		((Coord *)&LCD_Coord1)
#define Coordinate2		((Coord *)&LCD_Coord2)

void LCD::Command(uint32_t command) {
	this->csPin->Clear();
	this->spi.Send(command & 0xFF);
	this->csPin->Set();
}

void LCD::Data(uint32_t data) {
	this->csPin->Clear();
	this->spi.Send((0x100) | (data & 0xFF));
	this->csPin->Set();
}

void LCD::PutChar(char c, unsigned char * pFont) {
	unsigned int i, j;
	unsigned int cols;
	// unsigned int rows;
	unsigned int bytes;
	unsigned char pixelRow;
	unsigned int Word0;
	unsigned int Word1;
	unsigned char *pChar;

	cols = *pFont;
	// rows = *(pFont + 1);
	bytes = *(pFont + 2);
	pChar = pFont + (bytes * (c - 0x1F)) + bytes - 1;

	this->Command(RAMWR);
	for (i = 0; i < bytes; i++) {
		pixelRow = *pChar--;
		for (j = 0; j < cols / 2; j++) {
			Word0 = ((pixelRow & 0x1) != 0) ? WHITE : BLACK;
			pixelRow >>= 1;
			Word1 = ((pixelRow & 0x1) != 0) ? WHITE : BLACK;
			pixelRow >>= 1;
			this->Data((Word0 >> 4) & 0xFF);
			this->Data(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			this->Data(Word1 & 0xFF);
		}
	}
	this->Command(NOP);
}

LCD::LCD(PIN * csPin, PIN * resetPin) {
	Timer timer(SYSTICK);
	SPI_Config spi_config;

	spi_config.LSBF = 0;
	spi_config.SPIE = 0;
	spi_config.BITS = 9;
	spi_config.MASTER = 1;
	spi_config.CPHA = 1;
	spi_config.CPOL = 1;
	spi_config.SPCCR = 8;

	csPin->Mode(OUTPUT);
	resetPin->Mode(OUTPUT);

	this->spi.Init(&spi_config);
	this->csPin = csPin;

	resetPin->Clear();
	timer.DelayMS(100);
	resetPin->Set();
	timer.DelayMS(100);

	csPin->Clear();

	this->Command(DISCTL);
	this->Data(0x00);
	this->Data(0x20);
	this->Data(0x00);
	this->Command(COMSCN);
	this->Data(0x01);
	this->Command(OSCON);
	this->Command(SLPOUT);
	this->Command(PWRCTR);
	this->Data(0x0F);
	this->Command(DISNOR);
	timer.DelayMS(100);

	this->Command(DATCTL);
	this->Data(0x00);
	this->Data(0x00);
	this->Data(0x02);
	this->Command(VOLCTR);
	this->Data(0x28);
	this->Data(0x03);
	this->Command(NOP);
	timer.DelayMS(100);

	this->Command(DISON);

	csPin->Set();

	this->current_position.y = LINE_HEIGHT;
	this->current_position.x = START_AT;
}

void LCD::ClearArea(Coord * first, Coord * second) {
	int areaSize = first->size * second->size;

	this->Command(PASET);
	this->Data(first->position);
	this->Data(first->size);
	this->Command(CASET);
	this->Data(second->position);
	this->Data(second->size);

	this->Command(RAMWR);

	while (areaSize--) {
		this->Data(BLACK);
		this->Data(BLACK);
	}

	this->Command(NOP);
}

void LCD::ClearScreen() {
	Coordinate2->position = Coordinate1->position = 0;
	Coordinate2->size = Coordinate1->size = LCD_SIZE;

	this->ClearArea(Coordinate1, Coordinate2);
}

void LCD::Goto(uint32_t line, uint32_t column) {
	this->SetCursor(START_AT + column * COLUMN_WIDTH, line * LINE_HEIGHT);
}

void LCD::SetCursor(uint32_t x, uint32_t y) {

	if (x > WIDTH) {
		x = START_AT;
		y += LINE_HEIGHT;
	}

	// Invert coords;
	x = WIDTH - x;
	y = HEIGHT - y;

	this->Command(PASET);
	this->Data(y);
	this->Data(y + (COLUMN_WIDTH - 1));
	this->Command(CASET);
	this->Data(x);
	this->Data(x + (COLUMN_WIDTH - 1));
	this->current_position.x = WIDTH - x;
	this->current_position.y = HEIGHT - y;
}

void LCD::LineBreak() {
	this->current_position.y += LINE_HEIGHT;
	this->current_position.x = START_AT;
	this->SetCursor(START_AT, this->current_position.y);
}



void LCD::Write(char const * str) {
	while (*str) {

		while (*str && this->current_position.x == START_AT && *str == ' ') {
			str++;
		}

		if (*str == '\n') {
			this->LineBreak();
			str++;
		}

		this->PutChar(*str++, (unsigned char *) font8x8);
		this->SetCursor(this->current_position.x + COLUMN_WIDTH, this->current_position.y);
	}
}

void LCD::Writef(const char * format, ...) {
	char buffer[20];

	va_list args;
	va_start(args, format);
	vsprintf  (buffer, format, args);
	va_end(args);

	this->Write(buffer);
}

#endif /* LCD_ENABLE */
