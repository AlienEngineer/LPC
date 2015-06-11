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

#define WEEK_HOURS      (24*7)
#define ALARM_LOG_LEN	50

#define STATE_NONE		0
#define STATE_ALARM		1
#define STATE_TO_COLD	2
#define STATE_TO_HOT	3


class LogEntry {
public:
	DateTime Date;
	int8_t Temperature;
	bool HasData;
};


class Context {
public:
	Context();
	int8_t CurrentTemperature;

	int8_t LimitInf;
	int8_t LimitSup;
	RTC RealTimeClock;

	//
	// Log entries register
	LogEntry Entries[WEEK_HOURS];
	uint16_t EntryIndex;

	//
	// Alarm log register
	LogEntry Alarms[50];
	uint16_t AlarmIndex;

	//
	// When output [Alarm, Cool or Heat] happens.
	AutomaticResetEvent OutputEvent;

	//
	// Current State
	uint8_t State;

	bool TurnAlarmOff;

	void Log();
	void LogAlarm();
	void Config(int8_t limitInf, int8_t limitSup);
};

#endif /* CONTEXT_H_ */
