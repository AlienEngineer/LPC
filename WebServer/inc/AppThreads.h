/*
 * AppThreads.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef APPTHREADS_H_
#define APPTHREADS_H_

#include <Scheduler.h>
#include <Queue.h>


typedef struct {
	int8_t temperature;
	Queue<uint8_t> * temperatures;
} APP_DATA;

extern APP_DATA data;

/**
 * Code of Web Thread.
 */
void WebThread( void * pvParameters );

/**
 * Code of Temperature Thread
 */
void TemperatureThread( void * pvParameters );

#endif /* APPTHREADS_H_ */
