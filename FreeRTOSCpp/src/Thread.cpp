/*
 * Thread.cpp
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#include <Thread.h>

Thread::Thread(pdTASK_CODE pxTaskCode, const signed char * const pcName, void *pvParameters, unsigned portBASE_TYPE uxPriority) {
 this->result =	xTaskCreate(pxTaskCode, pcName, STACK_SIZE, pvParameters, uxPriority | portPRIVILEGE_BIT, &this->handle);
}

