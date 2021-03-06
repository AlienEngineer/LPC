#include <Context.h>

Context::Context() {
	this->State = STATE_NONE;
	TurnAlarmOff = false;

	this->EntryIndex = 0;
	this->AlarmIndex = 0;
	this->ButtonIndex = 0;
	this->ButtonConsumeIndex = 0;
	this->State = STATE_NONE;

	for (uint8_t i = 0; i < BT_LEN; ++i) {
		defaultButtonState.States[BT_LEN] = BT_NONE;
	}

	// BEGIN: Flash simulation!
	this->LimitInf = 10;
	this->LimitSup = 25;
	this->RealTimeClock.Config(2015, 6, 14, 18, 58, 0);

	for (uint8_t i = 18; i < 24; ++i) {
		this->CurrentTemperature = i;
		this->RealTimeClock.Config(2015, 6, 14, i, 15, 0);
		this->Log();
	}
	// END: Flash simulation!

	this->InputEvent.Wait();
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

	//
	// Notify listener
	InputEvent.Signal();
}

bool Context::HasButtons() {
	return (this->ButtonIndex != this->ButtonConsumeIndex);
}
ButtonsState * Context::PopButtonState() {

	if (this->ButtonConsumeIndex >= BT_STATE_LEN) {
		this->ButtonConsumeIndex = 0;
	}

	if (this->ButtonIndex == this->ButtonConsumeIndex) {
		return &defaultButtonState;
	}

	return &this->Buttons[this->ButtonConsumeIndex++];
}


ButtonsState * Context::GetLastButtonState() {

	return &this->Buttons[this->ButtonIndex-1];
}

ButtonsState * Context::PeekButtonState() {

	if (this->ButtonConsumeIndex >= BT_STATE_LEN) {
		this->ButtonConsumeIndex = 0;
	}

	if (this->ButtonIndex == this->ButtonConsumeIndex) {
		return &defaultButtonState;
	}

	return &this->Buttons[this->ButtonConsumeIndex];
}

bool ButtonsState::Up() {
	return this->States[BT_UP] == BT_PRESSED;
}

bool ButtonsState::Down() {
	return this->States[BT_DW] == BT_PRESSED;
}

bool ButtonsState::Ok() {
	return this->States[BT_OK] == BT_PRESSED;
}

bool ButtonsState::HasChanged(ButtonsState * state) {
	for (uint8_t i = 0; i < BT_LEN; ++i) {
			if (this->States[i] != state->States[i]) {
				return true;
			}
		}
	return false;
}

bool ButtonsState::Equals(ButtonsState * state) {
	return !this->HasChanged(state);
}

bool ButtonsState::AnyChange() {
	for (uint8_t i = 0; i < BT_LEN; ++i) {
		if (this->States[i] != BT_NONE) {
			return true;
		}
	}

	return false;
}

bool ButtonsState::AnyPressed() {
	for (uint8_t i = 0; i < BT_LEN; ++i) {
		if (this->States[i] == BT_PRESSED || this->States[i] == BT_LONGPRESS) {
			return true;
		}
	}

	return false;
}

bool ButtonsState::AnyReleased() {
	for (uint8_t i = 0; i < BT_LEN; ++i) {
		if (this->States[i] == BT_RELEASED) {
			return true;
		}
	}

	return false;
}

