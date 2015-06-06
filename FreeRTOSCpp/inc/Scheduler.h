/*
 * Scheduler.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <FreeRTOSCpp.h>

class Scheduler {
public:
	static void Start();
	static void Delay(uint32_t timeout);
};

#endif /* SCHEDULER_H_ */
