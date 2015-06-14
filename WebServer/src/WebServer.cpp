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

	Thread webThread(WebThread, (const signed char * const ) "Web", NULL, tskIDLE_PRIORITY);

	Thread temperatureThread(TemperatureThread, (const signed char * const ) "Temperature", NULL, tskIDLE_PRIORITY);

	Thread logRecordThread(LogRecordThread, (const signed char * const ) "LogRecord", NULL, tskIDLE_PRIORITY);

	Thread buttonsThread(ButtonsThread, (const signed char * const ) "Buttons", NULL, tskIDLE_PRIORITY);

	Thread menuThread(MenuThread, (const signed char * const ) "Menu", NULL, tskIDLE_PRIORITY);

	Scheduler::Start();
	return 0;
}
