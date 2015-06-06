/*
 * AppThreads.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef APPTHREADS_H_
#define APPTHREADS_H_

#include <Scheduler.h>

typedef struct {
	int8_t temperature;
} APP_DATA;

/**
 * Code of Web Thread.
 */
void WebThread( void * pvParameters );

/**
 * Code of Temperature Thread
 */
void TemperatureThread( void * pvParameters );

#endif /* APPTHREADS_H_ */
