/*
 * Monitor.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <FreeRTOSCpp.h>

class Monitor {
public:
	Monitor();
	void Wait();
	void Wait(uint32_t timeout);
protected:
	void ReleaseHandle();
	xSemaphoreHandle handle;
};

#endif /* MONITOR_H_ */
