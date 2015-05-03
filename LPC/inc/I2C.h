/*
 * I2C.h
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#ifndef I2C_H_
#define I2C_H_

#include <common.h>

class I2C {
public:
	I2C(LPC_I2C_TypeDef * i2c, uint32_t freq);
	uint32_t Transfer(uint8_t addr, int read, void *data, uint32_t size);
private:
	void Init();
	LPC_I2C_TypeDef * i2c;
	__IO uint32_t freq;
};

#endif /* I2C_H_ */
