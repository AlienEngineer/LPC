/*
 * Timer.h
 *
 *  Created on: 02/05/2015
 *      Author: Alien
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <common.h>

#define TIMER_MILLIS	(SystemCoreClock/1000)
#define TIMER_INTERVAL	(10)
#define SYSTICK		0


class Timer {
public:
	Timer(uint32_t timer);
	Timer();
	virtual void DelayMS(uint32_t millis);
	static uint32_t GetTickCount(uint32_t timer);
private:
	void Init(uint32_t timer);
	uint32_t timer;
};

#endif /* TIMER_H_ */

