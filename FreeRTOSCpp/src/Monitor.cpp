/*
 * Monitor.cpp
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#include <Monitor.h>

Monitor::Monitor() {

}

void Monitor::Wait() {
	xSemaphoreTake(this->handle, portMAX_DELAY);
}

void Monitor::Wait(uint32_t timeout) {
	xSemaphoreTake(this->handle, timeout);
}

void Monitor::ReleaseHandle() {
	xSemaphoreGive(this->handle);
}
