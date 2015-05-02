/*
 * Timer.cpp
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#include <Timer.h>

#define TIMER_MILLIS	(SystemCoreClock/1000)
#define TIMER_INTERVAL	(10)

__IO uint32_t systick_init = 0;
__IO uint32_t tick_count;

Timer::Timer(uint32_t timer) {
	this->timer = timer;

	switch(timer) {
	case SYSTICK:
		SysTick_Config(TIMER_MILLIS * TIMER_INTERVAL);
		tick_count = 0;
		systick_init = 1;
		break;
	}

}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}

void Timer::DelayMS(uint32_t millis) {
	switch(this->timer) {
	case SYSTICK:
	{
		__IO uint32_t curr = tick_count;

		// fix millis to match the same unit as elapsed.
		millis /= TIMER_INTERVAL;

		while((tick_count-curr) != millis ) { }
	}
	break;
	}
}

extern "C" {

void SysTick_Handler(void)
{
	tick_count++;
}

}
