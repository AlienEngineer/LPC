#include <LCD.h>
#include <Context.h>
#include <AppThreads.h>
#include <LPCTimer.h>

#define RESET		2
#define CS 			16

#define MENU_NONE		0
#define MENU_PRESENT	1

uint8_t MenuState = MENU_NONE;

void ShowCurrentTimeAndTemp(LCD * lcd) {
	DateTime * date = context.RealTimeClock.GetDate();
	YIELDEXEC(lcd->SetCursor(10, 20));
	YIELDEXEC(lcd->Writef("\n %04d/%02d/%02d\n %02d:%02d:%02d", date->Year,
					date->Month, date->Day, date->Hour, date->Minute,
					date->Second));
	YIELDEXEC(lcd->Writef("\n \n Temp: %d  ", context.CurrentTemperature));

	YIELDEXEC(lcd->Writef("\n Tmax: %d  ", context.LimitSup));
	YIELDEXEC(lcd->Writef("\n Tmin: %d  ", context.LimitInf));
}

void PrintMenu(LCD * lcd) {
	YIELDEXEC(lcd->ClearScreen());
	YIELDEXEC(lcd->SetCursor(10, 20));
	YIELDEXEC(lcd->Write("\n > Define Tmax"));
	YIELDEXEC(lcd->Write("\n > Define Tmin"));
	YIELDEXEC(lcd->Write("\n > Change Clock"));
	YIELDEXEC(lcd->Write("\n > Change Date"));
	YIELDEXEC(lcd->Write("\n > Temp log"));
	YIELDEXEC(lcd->Write("\n > Exit"));
}

uint8_t EvaluateState(LCD * lcd, ButtonsState* states) {

	switch (MenuState) {
	case MENU_NONE:
		//
		// Released a button or long pressed a button.
		if (states->Down() && states->Up()) {
			uint32_t time = Timer::GetTickCount(SYSTICK);

			do {
				ShowCurrentTimeAndTemp(lcd);
				Scheduler::Delay(1000);

				if (states->Down() && states->Up()) {
					if (Timer::GetElapsed(SYSTICK, time) >= 2000) {
						MenuState = MENU_PRESENT;
						PrintMenu(lcd);
						return MENU_PRESENT;
					}
				}
			}
			while((states = context.PopButtonState())->AnyPressed());

			Scheduler::Delay(4000);
			YIELDEXEC(lcd->ClearScreen());
			return MENU_NONE;
		}

		break;
	case MENU_PRESENT:


		break;
	}

	return MENU_NONE;
}

void MenuThread(void * pvParameters) {
	PIN csPin(0, CS);
	PIN resetPin(0, RESET);
	LCD lcd(&csPin, &resetPin);

	lcd.ClearScreen();
	lcd.SetCursor(10, 20);
	lcd.Write("\n     Welcome");
	lcd.Write("\n  Temp Monitor!");

	Scheduler::Delay(5000);
	lcd.ClearScreen();

	while (1) {
		context.InputEvent.Wait();

		//
		// Gets buttons states
		ButtonsState * states = context.PopButtonState();

		EvaluateState(&lcd, states);

	}

}
