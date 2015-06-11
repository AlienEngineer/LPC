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
#include <Context.h>

extern Context context;

/**
 * Code of Web Thread.
 */
void WebThread( void * pvParameters );

/**
 * Code of Temperature Thread
 */
void TemperatureThread( void * pvParameters );


void LogRecordThread( void * pvParameters );

void OutputThread( void * pvParameters );

#endif /* APPTHREADS_H_ */
