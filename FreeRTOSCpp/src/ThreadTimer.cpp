/*
 * ThreadTimer.cpp
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#include <ThreadTimer.h>
#include <Scheduler.h>

ThreadTimer::ThreadTimer() {
	// TODO Auto-generated constructor stub

}

void ThreadTimer::DelayMS(uint32_t millis) {
	Scheduler::Delay(millis);
}

