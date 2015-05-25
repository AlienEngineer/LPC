/*
 * Timer.cpp
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#include <Timers.h>

#define TIMER_MILLIS	(SystemCoreClock/1000)
#define TIMER_INTERVAL	(10)

__IO uint32_t systick_init = 0;
__IO uint32_t tick_count;

Timer::Timer(uint32_t timer) {
	this->Init(timer);
}

Timer::Timer() {
	this->Init(SYSTICK);
}

void Timer::Init(uint32_t timer) {
	this->timer = timer;

	switch (timer) {
	case SYSTICK:
		if (systick_init)
			return;

		SysTick_Config(TIMER_MILLIS * TIMER_INTERVAL);
		tick_count = 0;
		systick_init = 1;
		break;
	}

}

void Timer::DelayMS(uint32_t millis) {
	switch (this->timer) {
	case SYSTICK: {
		__IO uint32_t curr = tick_count;

		// fix millis to match the same unit as elapsed.
		millis /= TIMER_INTERVAL;

		while ((tick_count - curr) != millis) {
		}
	}
		break;
	}
}

uint32_t Timer::GetTickCount(uint32_t timer) {
	switch (timer) {
	case SYSTICK:
		return tick_count;
		break;
	}

	return 0;
}

extern "C" {

void SysTick_Handler(void) {
	tick_count++;
}

}
