/*
 * Thermometer.h
 *
 *  Created on: 03/05/2015
 *      Author: Alien
 */

#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#include <I2C.h>

class Thermometer {
public:
	Thermometer();
	int8_t GetTemperature();
private:
	I2C i2c;
};

#endif /* THERMOMETER_H_ */
