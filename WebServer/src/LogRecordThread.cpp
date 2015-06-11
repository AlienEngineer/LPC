
#include <AppThreads.h>

#define SECOND				1000
#define MINUTE				SECOND * 60

#define MONITOR_INTERVAL	1 * MINUTE

void LogRecordThread( void * pvParameters ) {
	uint8_t hour = 25;

	while(1) {
		DateTime * date = context.RealTimeClock.GetDate();

		if (hour != date->Hour) {
			LogEntry * logEntry = &context.Entries[context.EntryIndex];

			logEntry->Temperature = context.CurrentTemperature;
			logEntry->Date.Year = date->Year;
			logEntry->Date.Month = date->Month;
			logEntry->Date.Day = date->Day;
			logEntry->Date.Hour = date->Hour;
			logEntry->Date.Minute = date->Minute;
			logEntry->Date.Second = date->Second;
			logEntry->HasData = true;

			context.EntryIndex++;
			if (context.EntryIndex >= WEEK_HOURS) {
				context.EntryIndex = 0;
			}

			hour = date->Hour;
		}

		Scheduler::Delay(MONITOR_INTERVAL);
	}
}
