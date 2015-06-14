
#include <AppThreads.h>

#define SECOND				1000
#define MINUTE				SECOND * 60

#define MONITOR_INTERVAL	1 * MINUTE

void LogRecordThread( void * pvParameters ) {
	uint8_t hour = 25;

	while(1) {
		DateTime * date = context.RealTimeClock.GetDate();

		if (hour != date->Minute) {
			//
			// Logs the current datetime and temperature into a log entry.
			context.Log();

			hour = date->Minute;
		}

		Scheduler::Delay(MONITOR_INTERVAL);
	}
}
