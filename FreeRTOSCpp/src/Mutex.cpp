/*
 * Mutex.cpp
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#include <Mutex.h>

Mutex::Mutex() {
	this->handle = xSemaphoreCreateMutex();
}

void Mutex::Enter() {
	this->Wait();
}

void Mutex::Leave() {
	this->ReleaseHandle();
}
