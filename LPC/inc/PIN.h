/*
 * PIN.h
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#ifndef PIN_H_
#define PIN_H_

#include <common.h>

class PIN {
public:
	PIN(uint32_t port, uint32_t pin);
	virtual ~PIN();

	void SetFunction(uint32_t func);
private:
	__IO uint32_t port;
	__IO uint32_t pin;
};

#endif /* PIN_H_ */
