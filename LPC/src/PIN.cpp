/*
 * PIN.cpp
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#include <PIN.h>

#define PINSEL_Config(pinsel, pin, func) 	(pinsel |= (func << ((pin%16)*2)))

PIN::PIN(uint32_t port, uint32_t pin) {
	this->port = port;
	this->pin = pin;

}

void PIN::SetFunction(uint32_t func) {
	__IO uint32_t * base = ((&LPC_PINCON->PINSEL0) + (this->port * 2) + ((this->pin < 16 ? 0 : 1)));

	PINSEL_Config((*base), this->pin, func);
}
