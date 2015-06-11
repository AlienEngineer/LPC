/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <AppThreads.h>
#include <ConcurrentQueue.h>

//
// Shared app memory block.
Context context;

int main(void) {

	// Flash simulation!
	context.LimitInf = 10;
	context.LimitSup = 25;
	context.RealTimeClock.Config(2015, 6, 10, 18, 33, 0);

	Thread webThread(WebThread, (const signed char * const ) "Web", NULL,
			tskIDLE_PRIORITY);

	Thread temperatureThread(TemperatureThread,
			(const signed char * const ) "Temperature", NULL, tskIDLE_PRIORITY);

	Thread logRecordThread(LogRecordThread,
				(const signed char * const ) "LogRecord", NULL, tskIDLE_PRIORITY);

	Thread outputThread(OutputThread,
				(const signed char * const ) "Output", NULL, tskIDLE_PRIORITY);

	Scheduler::Start();
	return 0;
}
