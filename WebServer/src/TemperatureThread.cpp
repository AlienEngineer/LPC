
#include <Thermometer.h>
#include <AppThreads.h>

void TemperatureThread( void * pvParameters ) {
	Thermometer thermo;
	uint8_t temp;

	while(1) {

		temp = thermo.GetTemperature();

		data.temperature = temp;

		//
		// Enqueue temperature to be consumed by other threads.
		data.temperatures->Enqueue(temp);

		Scheduler::Delay(1000);

	}
}
