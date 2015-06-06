/*
 * AutomaticResetEvent.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef AUTOMATICRESETEVENT_H_
#define AUTOMATICRESETEVENT_H_
#include <FreeRTOSCpp.h>
#include <Monitor.h>

class AutomaticResetEvent: public Monitor {
public:
	AutomaticResetEvent();
	void Signal();
};

#endif /* AUTOMATICRESETEVENT_H_ */
