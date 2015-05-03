/*
 * PIN.h
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#ifndef PIN_H_
#define PIN_H_

#include <common.h>
#include <GPIO.h>

class PIN {
public:
	PIN();
	PIN(uint32_t port, uint32_t pin);
	void Init(uint32_t port, uint32_t pin);
	void SetFunction(uint32_t func);
	void Set();
	void Clear();
	void Mode(uint32_t mode);
private:
	__IO uint32_t port;
	__IO uint32_t pin;
	GPIO gpio;
};

#endif /* PIN_H_ */
