/*
 * GPIO.h
 *
 *  Created on: 26/04/2015
 *      Author: Alien
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "LPC17xx.h"
#include "core_cm3.h"
#include "Common.h"

class GPIO {
public:
	GPIO(LPC_GPIO_TypeDef * port);
	virtual ~GPIO();

	/**
	 * Set the pin mode for the given pin number.
	 * @param direction
	 * @param pin
	 */
	__INLINE void PinMode(uint32_t pin, uint32_t direction);

	uint32_t DigitalRead(uint32_t pin);
	__INLINE void DigitalWrite(uint32_t pin, uint32_t value);

private:
	LPC_GPIO_TypeDef * GPIOPx;

};

#endif /* GPIO_H_ */
