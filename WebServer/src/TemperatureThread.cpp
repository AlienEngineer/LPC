#include <Thermometer.h>
#include <PIN.h>
#include <AppThreads.h>
#include <LPCTimer.h>

uint32_t TurnOffAlarm(uint32_t lastTime, PIN& alarmPin) {
	//
	// Handle request to stop the alarm
	if (context.TurnAlarmOff && context.State == STATE_ALARM) {

		alarmPin.Clear();
		lastTime = Timer::GetTickCount(SYSTICK);
		context.State = STATE_NONE;

	} else {
		if (Timer::GetElapsed(0, lastTime) >= 5000) {
			context.TurnAlarmOff = false;
		}
	}
	return lastTime;
}

uint32_t TurnOnOutput(uint32_t lastTime, uint8_t temp, PIN& alarmPin) {
	if (!context.TurnAlarmOff) {
		if (Timer::GetElapsed(0, lastTime) >= 5000 && (context.State == STATE_TO_COLD || context.State == STATE_TO_HOT)) {

			context.State = STATE_ALARM;
			context.LogAlarm();
			alarmPin.Set();

		} else if (context.State == STATE_NONE && temp > context.LimitSup) {

			context.State = STATE_TO_HOT;
			lastTime = Timer::GetTickCount(SYSTICK);

			// alarmPin.Set();	// TODO: Switch to proper pin out

		} else if (context.State == STATE_NONE && temp < context.LimitInf) {

			context.State = STATE_TO_COLD;
			lastTime = Timer::GetTickCount(SYSTICK);

			// alarmPin.Set();	// TODO: Switch to proper pin out

		} else if (context.State != STATE_NONE && temp >= context.LimitInf && temp <= context.LimitSup) {
			context.TurnAlarmOff = true;
		}
	}
	return lastTime;
}

void TemperatureThread(void * pvParameters) {
	Thermometer thermo;
	uint8_t temp;
	PIN alarmPin(0, 22);
	alarmPin.Mode(OUTPUT);
	uint32_t lastTime = 0;

	while (1) {

		temp = thermo.GetTemperature();

		context.CurrentTemperature = temp;

		//
		// Handle request to stop the alarm
		lastTime = TurnOffAlarm(lastTime, alarmPin);

		//
		// Verifies if the temperature is out of bounds and turns on cooling, heat or alarm
		lastTime = TurnOnOutput(lastTime, temp, alarmPin);

		Scheduler::Delay(1000);

	}
}

