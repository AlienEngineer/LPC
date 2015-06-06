/*
 * Thread.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef THREAD_H_
#define THREAD_H_
#include <FreeRTOSCpp.h>

class Thread {
public:
	Thread(pdTASK_CODE pxTaskCode, const signed char * const pcName, void *pvParameters, unsigned portBASE_TYPE uxPriority);
private:
	xTaskHandle handle;
};

#endif /* THREAD_H_ */
