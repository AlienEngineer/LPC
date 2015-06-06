/*
 * ThreadTimer.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef THREADTIMER_H_
#define THREADTIMER_H_

#include <LPCTimer.h>
#include <FreeRTOSCpp.h>

class ThreadTimer : public Timer {
public:
	ThreadTimer();
	void DelayMS(uint32_t millis);
};

#endif /* THREADTIMER_H_ */
