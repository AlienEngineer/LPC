/*
 * Button.h
 *
 *  Created on: 13/06/2015
 *      Author: sergio.ferreira
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <common.h>
#include <PIN.h>


#define BT_NONE 		0
#define BT_PRESSED		1
#define BT_RELEASED		2
#define BT_LONGPRESS	3

class Button {
public:
	Button(uint32_t port, uint32_t pin, uint32_t longDelta);
	void Evaluate();
	uint8_t state;
private:
	PIN btPin;
	uint32_t timestamp;
	uint32_t longDelta;
};

#endif /* BUTTON_H_ */
