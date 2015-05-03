/*
 * GPIO.h
 *
 *  Created on: 01/05/2015
 *      Author: Alien
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <common.h>


class GPIO {
public:
	GPIO(LPC_GPIO_TypeDef * port);

	/**
	 * Set the pin mode for the given pin number.
	 * @param direction
	 * @param pin
	 */
	void PinMode(uint32_t pin, uint32_t direction);

	/**
	 * Reads from the given pin. Returns HIGH or LOW constant.
	 * @param pin
	 */
	uint32_t DigitalRead(uint32_t pin);

	/**
	 * Set the pin value with the given value. Use HIGH or LOW constants.
	 * @param pin
	 * @param value
	 */
	void DigitalWrite(uint32_t pin, uint32_t value);

private:
	LPC_GPIO_TypeDef * GPIOPx;

};

#endif /* GPIO_H_ */
