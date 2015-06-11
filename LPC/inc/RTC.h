/*
 * RTC.h
 *
 *  Created on: 10/06/2015
 *      Author: sergio.ferreira
 */

#ifndef RTC_H_
#define RTC_H_

#include <common.h>

class DateTime {
public:
	DateTime();
	DateTime(uint16_t year, uint16_t month, uint16_t day);
	DateTime(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second);

	__IO uint16_t Year;
	__IO uint16_t Month;
	__IO uint16_t Day;
	__IO uint8_t Hour;
	__IO uint8_t Minute;
	__IO uint8_t Second;


	uint32_t TimeToSeconds();
	void CopyTo(DateTime * date);

};

class RTC {
public:
	RTC();
	RTC(DateTime * datetime);
	RTC(uint16_t year, uint16_t month, uint16_t day);
	RTC(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second);
	void Config(DateTime * datetime);
	void Config(uint16_t year, uint16_t month, uint16_t day);
	void Config(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second);
	DateTime * GetDate();
private:
	void Refresh();
	DateTime datetime;
};

#endif /* RTC_H_ */
