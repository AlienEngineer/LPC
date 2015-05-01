/*
 * GPIO.cpp
 *
 *  Created on: 01/05/2015
 *      Author: Alien
 */

#include <GPIO.h>

GPIO::GPIO(LPC_GPIO_TypeDef * port) {
	this->GPIOPx = port;
}

GPIO::~GPIO() {

}
void GPIO::PinMode(uint32_t pin, uint32_t direction)
{
	pin = (1 << pin);

	if(direction == INPUT)
		this->GPIOPx->FIODIR &= ~pin;
	else if(direction == OUTPUT)
		this->GPIOPx->FIODIR |= pin;
}

void GPIO::DigitalWrite(uint32_t pin, uint32_t value)
{
	pin = (1 << pin);

	if (value == HIGH) {
		this->GPIOPx->FIOSET = pin;
	}
	else {
		this->GPIOPx->FIOCLR = pin;
	}
}

uint32_t GPIO::DigitalRead(uint32_t pin) {
	return (this->GPIOPx->FIOPIN & (1 << pin)) ? HIGH : LOW;
}
