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

//
// Shared app memory block.
APP_DATA data;


int main(void) {

	Thread webThread(WebThread, (const signed char * const)"Web", &data, tskIDLE_PRIORITY);
	Thread temperatureThread(TemperatureThread, (const signed char * const)"Temperature", &data, tskIDLE_PRIORITY);

	Scheduler::Start();
	return 0;
}

