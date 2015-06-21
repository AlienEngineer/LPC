#include <LCD.h>
#include <Context.h>
#include <AppThreads.h>
#include <LPCTimer.h>
#include <Menu.h>

#define RESET		2
#define CS 			16

PIN csPin(0, CS);
PIN resetPin(0, RESET);
LCD lcd(&csPin, &resetPin);
Menu menu(&lcd);
ButtonsState * currentStates;

void ShowCurrentTimeAndTemp() {
	DateTime * date = context.RealTimeClock.GetDate();
	YIELDEXEC(lcd.SetCursor(10, 20));
	YIELDEXEC(
			lcd.Writef("\n %04d/%02d/%02d\n %02d:%02d:%02d", date->Year,
					date->Month, date->Day, date->Hour, date->Minute,
					date->Second));
	YIELDEXEC(lcd.Writef("\n \n Temp: %d  ", context.CurrentTemperature));

	YIELDEXEC(lcd.Writef("\n Tmax: %d  ", context.LimitSup));
	YIELDEXEC(lcd.Writef("\n Tmin: %d  ", context.LimitInf));
}

void EvaluateState() {

	switch (menu.GetMenuState()) {
		case MENU_NONE:
			//
			// Released a button or long pressed a button.
			if (currentStates->Down() && currentStates->Up()) {
				uint32_t time = Timer::GetTickCount(SYSTICK);

				do {
					ShowCurrentTimeAndTemp();

					Scheduler::Delay(1000);
					currentStates = context.PeekButtonState();

					if (currentStates->Down() && currentStates->Up()) {
						if (Timer::GetElapsed(SYSTICK, time) >= 2000) {
							menu.MoveTo(MENU_PRESENT);
							menu.Render();
							return;
						}
					}

				} while (!(currentStates = context.PopButtonState())->AnyReleased());

				// TODO: while here to see if a D+U was pressed again to restart analysis!
				Scheduler::Delay(4000);
				YIELDEXEC(lcd.ClearScreen());
			} else if (currentStates->Ok() && context.State == STATE_ALARM) {
				context.TurnAlarmOff = true;
			}

			break;
		default:
			if (currentStates->Down() && currentStates->Up()) {
				break;
			}

			if (currentStates->Down()) {
				menu.GoDown();
			} else if (currentStates->Up()) {
				menu.GoUp();
			} else if (currentStates->Ok()) {
				menu.Enter();
			}

			break;

	}
}



void MenuThread(void * pvParameters) {

	lcd.ClearScreen();
	lcd.SetCursor(10, 20);
	lcd.Write("\n     Welcome");
	lcd.Write("\n  Temp Monitor!");

	Scheduler::Delay(2500);
	lcd.ClearScreen();

	while (1) {
		context.InputEvent.Wait();

		while (context.HasButtons()) {

			//
			// Gets buttons states
			currentStates = context.PopButtonState();

			EvaluateState();
		}
	}

}
