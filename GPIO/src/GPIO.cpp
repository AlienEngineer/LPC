/*
 * GPIO.cpp
 *
 *  Created on: 26/04/2015
 *      Author: Alien
 */

#include "GPIO.h"

GPIO::GPIO(LPC_GPIO_TypeDef * port) {
	this->GPIOPx = port;
}

GPIO::~GPIO() {

}

__INLINE void GPIO::PinMode(uint32_t pin, uint32_t direction)
{
	this->GPIOPx->FIODIR = (direction == INPUT) ?
			this->GPIOPx->FIODIR & ~pin :
			this->GPIOPx->FIODIR | pin;
}

void GPIO::DigitalWrite(uint32_t pin, uint32_t value) {
	if (value == HIGH) {
		this->GPIOPx->FIOSET = pin;
	}
	else {
		this->GPIOPx->FIOCLR |= pin;
	}
}

__INLINE uint32_t GPIO::DigitalRead(uint32_t pin) {
	return (this->GPIOPx->FIOPIN & pin) ? HIGH : LOW;
}
