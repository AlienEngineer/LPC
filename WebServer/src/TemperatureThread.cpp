#include <Thermometer.h>
#include <PIN.h>
#include <AppThreads.h>
#include <LPCTimer.h>

#define TIME_BEFORE_SIGNAL 1000 * 60
#define MONITOR_INTERVAL 1000 * 1
PIN alarmPin(0, 22);
PIN coolPin(2, 3);
PIN heatPin(2, 4);

uint32_t TurnOffAlarm(uint32_t lastTime) {
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

uint32_t TurnOnOutput(uint32_t lastTime, uint8_t temp) {
	if (!context.TurnAlarmOff) {
		if (Timer::GetElapsed(0, lastTime) >= TIME_BEFORE_SIGNAL && (context.State == STATE_TO_COLD || context.State == STATE_TO_HOT)) {

			context.State = STATE_ALARM;
			context.LogAlarm();
			alarmPin.Set();

		} else if (context.State == STATE_NONE && temp > context.LimitSup) {

			context.State = STATE_TO_HOT;
			lastTime = Timer::GetTickCount(SYSTICK);

			heatPin.Set();

		} else if (context.State == STATE_NONE && temp < context.LimitInf) {

			context.State = STATE_TO_COLD;
			lastTime = Timer::GetTickCount(SYSTICK);

			coolPin.Set();

		} else if (temp >= context.LimitInf && temp <= context.LimitSup) {
			lastTime = Timer::GetTickCount(SYSTICK);
			context.TurnAlarmOff = context.State != STATE_NONE;
			context.State = STATE_NONE;
			coolPin.Clear();
			heatPin.Clear();
		}
	}
	return lastTime;
}

void TemperatureThread(void * pvParameters) {
	Thermometer thermo;
	uint8_t temp;
	alarmPin.Mode(OUTPUT);
	coolPin.Mode(OUTPUT);
	heatPin.Mode(OUTPUT);
	uint32_t lastTime = 0;

	while (1) {

		temp = thermo.GetTemperature();

		context.CurrentTemperature = temp;

		//
		// Handle request to stop the alarm
		lastTime = TurnOffAlarm(lastTime);

		//
		// Verifies if the temperature is out of bounds and turns on cooling, heat or alarm
		lastTime = TurnOnOutput(lastTime, temp);

		Scheduler::Delay(MONITOR_INTERVAL);
	}
}

