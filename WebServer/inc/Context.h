/*
 * Context.h
 *
 *  Created on: 09/06/2015
 *      Author: sergio.ferreira
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <RTC.h>
#include <AutomaticResetEvent.h>

#define WEEK_HOURS    (24*7)

class LogEntry {
public:
	DateTime Date;
	int8_t Temperature;
	bool HasData;
};


class Context {
public:
	int8_t CurrentTemperature;

	int8_t LimitInf;
	int8_t LimitSup;
	RTC RealTimeClock;

	LogEntry Entries[WEEK_HOURS];
	uint16_t EntryIndex;

	AutomaticResetEvent OutputEvent;
};

#endif /* CONTEXT_H_ */
