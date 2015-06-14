#include <Context.h>

Context::Context() {
	this->State = STATE_NONE;
	TurnAlarmOff = false;

	// Flash simulation!
	this->LimitInf = 10;
	this->LimitSup = 25;
	// this->RealTimeClock.Config(2015, 6, 10, 18, 33, 0);
}

void Context::Log() {
	DateTime * date = this->RealTimeClock.GetDate();
	LogEntry * logEntry = &this->Entries[this->EntryIndex];

	date->CopyTo(&logEntry->Date);

	logEntry->Temperature = this->CurrentTemperature;
	logEntry->HasData = true;

	this->EntryIndex++;
	if (this->EntryIndex >= WEEK_HOURS) {
		this->EntryIndex = 0;
	}
}

void Context::LogAlarm() {
	DateTime * date = this->RealTimeClock.GetDate();
	LogEntry * logEntry = &this->Alarms[this->AlarmIndex];

	date->CopyTo(&logEntry->Date);

	logEntry->Temperature = this->CurrentTemperature;
	logEntry->HasData = true;

	this->AlarmIndex++;
	if (this->AlarmIndex >= WEEK_HOURS) {
		this->AlarmIndex = 0;
	}
}

void Context::Config(int8_t limitInf, int8_t limitSup) {

	if (limitInf > limitSup) {
		return;
	}

	this->LimitInf = limitInf;
	this->LimitSup = limitSup;
}

void Context::SaveButtonState(ButtonsState * buttonState) {

	for(uint8_t i = 0; i < BT_LEN; ++i) {
		this->Buttons[this->ButtonIndex].States[i] = buttonState->States[i];
		buttonState->States[i] = BT_NONE;
	}

	this->ButtonIndex++;

	if (this->ButtonIndex > BT_STATE_LEN) {
		this->ButtonIndex = 0;
	}

}
