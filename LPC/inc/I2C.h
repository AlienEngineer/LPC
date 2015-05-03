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
	I2C();
	uint32_t Transfer(uint8_t addr, uint8_t read, void *data, uint32_t size);
	void Init(LPC_I2C_TypeDef * i2c, uint32_t frequency);
private:
	void InitSdaScl(uint32_t sdaPin, uint32_t sclPin, uint32_t func);
	void Init();
	LPC_I2C_TypeDef * i2c;
	__IO uint32_t freq;
};

#endif /* I2C_H_ */
