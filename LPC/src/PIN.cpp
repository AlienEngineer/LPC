/*
 * PIN.cpp
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#include <PIN.h>



PIN::PIN(uint32_t port, uint32_t pin) {
	this->Init(port, pin);
}

PIN::PIN() { }

void PIN::Init(uint32_t port, uint32_t pin) {
	this->port = port;
	this->pin = pin;
	this->gpio.Init(((LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE) + port));
}

void PIN::SetFunction(uint32_t func) {
	SetFunction(this->port, this->pin, func);
}

void PIN::SetFunction(uint32_t port, uint32_t pin, uint32_t func) {
	__IO uint32_t * base = PINSEL_BASE(port, pin);

	PINSEL_Config((*base), pin, func);
}

bool PIN::IsOn() {
	return this->gpio.DigitalRead(this->pin) == HIGH;
}
void PIN::Set() {
	this->gpio.DigitalWrite(this->pin, HIGH);
}

void PIN::Clear() {
	this->gpio.DigitalWrite(this->pin, LOW);
}

void PIN::Mode(uint32_t mode) {
	this->gpio.PinMode(this->pin, mode);
}
