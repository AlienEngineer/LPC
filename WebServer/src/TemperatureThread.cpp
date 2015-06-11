
#include <Thermometer.h>
#include <PIN.h>
#include <AppThreads.h>

void TemperatureThread( void * pvParameters ) {
	Thermometer thermo;
	uint8_t temp;

	while(1) {

		temp = thermo.GetTemperature();

		context.CurrentTemperature = temp;

		if (temp > context.LimitSup || temp < context.LimitInf) {
			context.OutputEvent.Signal();
		}

		Scheduler::Delay(1000);

	}
}


void OutputThread( void * pvParameters ) {
	PIN alarmPin(0, 22);

	while(1) {

		context.OutputEvent.Wait();

		alarmPin.Set();

	}
}
