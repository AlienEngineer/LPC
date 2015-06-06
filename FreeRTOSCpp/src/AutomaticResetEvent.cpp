/*
 * AutomaticResetEvent.cpp
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#include <AutomaticResetEvent.h>

AutomaticResetEvent::AutomaticResetEvent() {
	vSemaphoreCreateBinary(this->handle);
}

void AutomaticResetEvent::Signal() {
	this->ReleaseHandle();
}
