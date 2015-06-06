/*
 * Mutex.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <FreeRTOSCpp.h>
#include <Monitor.h>

class Mutex: public Monitor {
public:
	Mutex();
	void Enter();
	void Leave();
private:
	xSemaphoreHandle handle;
};

#endif /* MUTEX_H_ */
