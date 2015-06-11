/*
 * RTC.cpp
 *
 *  Created on: 10/06/2015
 *      Author: sergio.ferreira
 */

#include <RTC.h>

#define RESET 		(1<<1)
#define START 		(1<<0)

RTC::RTC() {

}
RTC::RTC(DateTime * datetime) {
	this->Config(datetime);
}
RTC::RTC(uint16_t year, uint16_t month, uint16_t day) {
	this->Config(year, month, day, 0, 0, 0);
}
RTC::RTC(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	this->Config(year, month, day, hour, minute, second);
}

void RTC::Config(uint16_t year, uint16_t month, uint16_t day) {
	this->Config(year, month, day, 0, 0, 0);
}

void RTC::Config(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	this->datetime.Year = year;
	this->datetime.Month = month;
	this->datetime.Day = day;
	this->datetime.Hour = hour;
	this->datetime.Minute = minute;
	this->datetime.Second = second;

	this->Config(&this->datetime);
}


void RTC::Config(DateTime * datetime) {
	LPC_RTC->CCR = RESET;

	LPC_RTC->CCR = LPC_RTC->AMR = LPC_RTC->CIIR = LPC_RTC->ILR = 0;

	LPC_RTC->SEC = datetime->Second;
	LPC_RTC->MIN = datetime->Minute;
	LPC_RTC->HOUR = datetime->Hour;
	LPC_RTC->DOM = datetime->Day;
	LPC_RTC->MONTH = datetime->Month;
	LPC_RTC->YEAR = datetime->Year;

	this->Refresh();

	LPC_RTC->CCR &= ~RESET;
	LPC_RTC->CCR |= START;
}

DateTime * RTC::GetDate() {
	this->Refresh();
	return &this->datetime;
}

void RTC::Refresh() {
	this->datetime.Second = LPC_RTC->SEC;
	this->datetime.Minute = LPC_RTC->MIN;
	this->datetime.Hour = LPC_RTC->HOUR;
	this->datetime.Day = LPC_RTC->DOM;
	this->datetime.Month = LPC_RTC->MONTH;
	this->datetime.Year = LPC_RTC->YEAR;

	if (this->datetime.Second != LPC_RTC->SEC) {
		Refresh();
	}
}

DateTime::DateTime() {}

DateTime::DateTime(uint16_t year, uint16_t month, uint16_t day) {
	this->Year = year;
	this->Month = month;
	this->Day = day;
}
DateTime::DateTime(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	this->Year = year;
	this->Month = month;
	this->Day = day;
	this->Hour = hour;
	this->Minute = minute;
	this->Second = second;
}

uint32_t DateTime::TimeToSeconds() {
	return this->Second + (this->Minute * 60) + (this->Hour * 60 * 60);
}
