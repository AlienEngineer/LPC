/*
 * SPI.h
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#ifndef SPI_H_
#define SPI_H_

#include <common.h>

typedef struct {
	uint32_t	RESERVED0;
	uint32_t	BIT_ENABLE;
	uint32_t	CPHA;
	uint32_t	CPOL;
	uint32_t	MASTER;
	uint32_t	LSBF;
	uint32_t	SPIE;
	uint32_t	BITS;
	uint32_t 	SPCCR;
} SPI_Config;

class SPI {
public:
	SPI(SPI_Config * config);
	SPI();
	void Init(SPI_Config * config);
	void Send(uint32_t value);
};


#endif /* SPI_H_ */
