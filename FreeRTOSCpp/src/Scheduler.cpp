/*
 * Scheduler.cpp
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#include <Scheduler.h>

void Scheduler::Start() {
	vTaskStartScheduler();
}

void Scheduler::Delay(uint32_t timeout) {
	vTaskDelay(timeout);
}

void Scheduler::Yield() {
	taskYIELD();
}
