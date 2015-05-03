/*
 * SPI.cpp
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#include <SPI.h>
#include <PIN.h>

#define DEFAULT_SPCCR 		(8)

#define SPI_STATUS_ABRT		(1 << 3)
#define SPI_STATUS_MODF		(1 << 4)
#define SPI_STATUS_ROVR		(1 << 5)
#define SPI_STATUS_WCOL		(1 << 6)
#define SPI_STATUS_SPIF		(1 << 7)

#define SPI_SCK			15
#define SPI_MISO		17
#define SPI_MOSI		18
#define PINSEL_SPI		0x3

void SPI::Init(SPI_Config * config) {
	PIN miso(0, SPI_MISO);
	PIN mosi(0, SPI_MOSI);
	PIN sck(0, SPI_SCK);

	miso.SetFunction(PINSEL_SPI);
	mosi.SetFunction(PINSEL_SPI);
	sck.SetFunction(PINSEL_SPI);

	// Make sure bit enable is true if more than 8 bits is used.
	config->BIT_ENABLE = (config->BITS > 8);

	// Use default value if none was specified.
	if (config->SPCCR == 0) {
		config->SPCCR = DEFAULT_SPCCR;
	}

	LPC_SPI->SPCCR = config->SPCCR;
	LPC_SPI->SPCR = ((config->BIT_ENABLE & 0x1) << 2)
			| ((config->CPHA & 0x1) << 3) | ((config->CPOL & 0x1) << 4)
			| ((config->MASTER & 0x1) << 5) | ((config->LSBF & 0x1) << 6)
			| ((config->SPIE & 0x1) << 7) | ((config->BITS & 0xF) << 8);
}

SPI::SPI(SPI_Config * config) {
	this->Init(config);
}

SPI::SPI() {

}

void SPI::Send(uint32_t value) {
	LPC_SPI->SPDR = value;

	while (1) {
		if (LPC_SPI->SPSR == SPI_STATUS_SPIF)
			break;
	}
}
