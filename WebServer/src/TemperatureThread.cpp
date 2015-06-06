
#include <Thermometer.h>
#include <AppThreads.h>

void TemperatureThread( void * pvParameters ) {
	Thermometer thermo;

	while(1) {

		data.temperature = thermo.GetTemperature();
		Scheduler::Delay(1000);

	}
}
