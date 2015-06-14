/*
 * Button.cpp
 *
 *  Created on: 13/06/2015
 *      Author: sergio.ferreira
 */

#include <Button.h>
#include <LPCTimer.h>



Button::Button(uint32_t port, uint32_t pin, uint32_t longDelta) {
	btPin.Init(port, pin);
	this->longDelta = longDelta;
}

void Button::Evaluate() {
	bool pressed = !btPin.IsOn();

	switch (this->state) {
		case BT_NONE:
			if (pressed) {
				this->timestamp = Timer::GetTickCount(SYSTICK);
				state = BT_PRESSED;
			}

			break;
		case BT_PRESSED:
			if (!pressed) {
				state = BT_RELEASED;
			}

			if (Timer::GetElapsed(SYSTICK, this->timestamp) >= this->longDelta) {
				state = BT_LONGPRESS;
			}

			break;
		case BT_LONGPRESS:
		case BT_RELEASED:
			state = BT_NONE;
			break;
	}

}
