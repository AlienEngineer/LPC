#include <Menu.h>
#include <AppThreads.h>

Menu::Menu(LCD * lcd) {
	this->lcd = lcd;
	MenuState = MENU_NONE;
	MenuSelection = 0;
	MenuLastSelection = 0;
}

uint8_t Menu::GetMenuState() {
	return MenuState;
}

void Menu::SwitchMenuToSelection(uint8_t index) {
	MenuLastSelection = MenuSelection;
	MenuSelection = index;
	this->RenderMenuSelection();
}

int32_t Limit(int32_t value, int32_t inf, int32_t sup) {
	if (value < inf) {
		return sup;
	} else if (value > sup) {
		return inf;
	}
	return value;
}

void Menu::GoDown() {
	TempValue[TempIndex]--;

	switch(MenuState) {
		case MENU_PRESENT:
			if (MenuSelection < MENU_MAX) {
				SwitchMenuToSelection(MenuSelection + 1);
			}
		break;

		case MENU_DEF_CLOCK:
			switch (TempIndex) {
				case 0: /*HOUR*/
					TempValue[0] = Limit(TempValue[0], 0, 23);
					break;
				case 1:
				case 2:
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 0, 59);
					break;
			}
			this->RenderTempValue();
			break;
		case MENU_DEF_DATE:
			switch (TempIndex) {
				case 1:
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 1, 12);
				case 2:
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 1, 31);
					break;
			}
			this->RenderTempValue();
			break;
		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
			this->RenderTempValue();
			break;
		case MENU_SHOW_TEMP:
			// Fix common decrement.
			TempValue[TempIndex]++;

			// don't go down 4 ever.
			if (!context.Entries[TempValue[0]].HasData) {
				return;
			}
			this->Render();
			break;
	}

}


void Menu::GoUp() {
	TempValue[TempIndex]++;

	switch(MenuState) {
		case MENU_PRESENT:
			if (MenuSelection > MENU_MIN) {
				SwitchMenuToSelection(MenuSelection - 1);
			}
			break;

		case MENU_DEF_CLOCK:
			switch(TempIndex) {
				case 0: /*HOUR*/
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 0, 23);
					break;
				case 1:
				case 2:
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 0, 59);
					break;
			}
			this->RenderTempValue();
			break;
		case MENU_DEF_DATE:
			switch (TempIndex) {
				case 1:
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 1, 12);
				case 2:
					TempValue[TempIndex] = Limit(TempValue[TempIndex], 1, 31);
					break;
			}
			this->RenderTempValue();
			break;
		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
			this->RenderTempValue();
			break;
		case MENU_SHOW_TEMP:
			// Fix common increment.
			TempValue[TempIndex]--;

			// move a page up.
			TempValue[0] += LOG_VIEW_PAGE_SIZE;

			if (TempValue[0] > WEEK_HOURS) {
				TempValue[0] = TempValue[0] - WEEK_HOURS;
			}

			// don't go up 4 ever. Tries to find the first valid entry.
			while (!context.Entries[TempValue[0]].HasData) {
				TempValue[0]--;
			}

			this->Render();
			break;
	}
}

void Menu::Enter() {
	switch(MenuState) {
		case MENU_PRESENT:
			this->HandleSelection();
			break;
		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
		case MENU_DEF_CLOCK:
		case MENU_DEF_DATE:
		case MENU_SHOW_TEMP:
			this->HandleTempValueEnter();
			break;
	}
	this->Render();
}

/**
 * Handler that is called when ok button is called.
 */
void Menu::HandleTempValueEnter() {
	TempIndex++;
	DateTime * date =  context.RealTimeClock.GetDate();

	switch (MenuSelection) {
		case DEF_TMAX:
			TempIndex = 0;
			context.Config(context.LimitInf, TempValue[0]);
			break;
		case DEF_TMIN:
			TempIndex = 0;
			context.Config(TempValue[0], context.LimitSup);
			break;
		case DEF_CLOCK:

			if (TempIndex < 3) {
				return; // Avoid being sent back in menu.
			} else {
				context.RealTimeClock.Config(
						date->Year, date->Month, date->Day,
						TempValue[0], TempValue[1], TempValue[2]
						);
			}
			break;
		case DEF_DATE:

			if (TempIndex < 3) {
				return; // Avoid being sent back in menu.
			} else {
				context.RealTimeClock.Config(
						TempValue[0], TempValue[1], TempValue[2],
						date->Hour, date->Minute, date->Second
						);
			}
			break;
	}

	// GO BACK
	this->MoveTo(MENU_PRESENT);

}

void Menu::HandleSelection() {
	TempIndex = 0;
	DateTime * date =  context.RealTimeClock.GetDate();

	switch (MenuSelection) {
		case DEF_TMAX:
			TempValue[0] = context.LimitSup;
			this->MoveTo(MENU_DEF_TMAX);
			break;
		case DEF_TMIN:
			TempValue[0] = context.LimitInf;
			this->MoveTo(MENU_DEF_TMIN);
			break;
		case DEF_CLOCK:
			TempValue[0] = date->Hour;
			TempValue[1] = date->Minute;
			TempValue[2] = date->Second;
			this->MoveTo(MENU_DEF_CLOCK);
			break;
		case DEF_DATE:
			TempValue[0] = date->Year;
			TempValue[1] = date->Month;
			TempValue[2] = date->Day;
			this->MoveTo(MENU_DEF_DATE);
			break;
		case SHOW_TEMP:
			TempValue[0] = context.EntryIndex-1;
			this->MoveTo(MENU_SHOW_TEMP);
			break;
		case EXIT:
			this->MoveTo(MENU_NONE);
			break;
	}

}

void Menu::MoveTo(uint8_t state) {
	MenuState = state;
}

void Menu::Render() {
	YIELDEXEC(lcd->ClearScreen());
	YIELDEXEC(lcd->Goto(1, 1));

	switch (MenuState) {
		case MENU_PRESENT:
			YIELDEXEC(lcd->Write("\n   Define Tmax"));
			YIELDEXEC(lcd->Write("\n   Define Tmin"));
			YIELDEXEC(lcd->Write("\n   Change Clock"));
			YIELDEXEC(lcd->Write("\n   Change Date"));
			YIELDEXEC(lcd->Write("\n   Temp log"));
			YIELDEXEC(lcd->Write("\n   Exit"));
			this->SwitchMenuToSelection(0);
			break;
		case MENU_DEF_TMAX:
			YIELDEXEC(lcd->Write("\n  Define Tmax:"));
			YIELDEXEC(lcd->Writef("\n  %d degrees", TempValue[0]));
			break;
		case MENU_DEF_TMIN:
			YIELDEXEC(lcd->Write("\n  Define Tmin:"));
			YIELDEXEC(lcd->Writef("\n  %d degrees", TempValue[0]));
			break;
		case MENU_DEF_CLOCK:
			YIELDEXEC(lcd->Write("\n  Define Clock:"));
			YIELDEXEC(lcd->Writef("\n  %02d:%02d:%02d", TempValue[0], TempValue[1], TempValue[2]));
			break;
		case MENU_DEF_DATE:
			YIELDEXEC(lcd->Write("\n  Define Date:"));
			YIELDEXEC(lcd->Writef("\n  %04d/%02d/%02d", TempValue[0], TempValue[1], TempValue[2]));
			break;
		case MENU_SHOW_TEMP:
			uint8_t line = 1;
			while(context.Entries[TempValue[0]].HasData && line <= LOG_VIEW_PAGE_SIZE) {
				LogEntry * entry = &context.Entries[TempValue[0]];
				YIELDEXEC(lcd->Goto(line++, 0));
				YIELDEXEC(lcd->Writef("%04d/%02d/%02d %02d:%02d",
						entry->Date.Year, entry->Date.Month, entry->Date.Day,
						entry->Date.Hour, entry->Date.Minute));
				YIELDEXEC(lcd->Goto(line++, 0));
				YIELDEXEC(lcd->Writef("%02d degrees", entry->Temperature));


				TempValue[0]--;
				if (TempValue[0] < 0) {
					TempValue[0] = WEEK_HOURS;
				}
			}

			if (context.Entries[TempValue[0]].HasData) {
				YIELDEXEC(lcd->Goto(line++, 0));
				YIELDEXEC(lcd->Write("..."));
			}

			break;
	}
}

void Menu::RenderTempValue() {
	YIELDEXEC(lcd->Goto(2, 1));
	switch (MenuState) {
		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
			YIELDEXEC(lcd->Writef("\n  %d degrees    ", TempValue[0]));
			break;
		case MENU_DEF_CLOCK:
			YIELDEXEC(lcd->Writef("\n  %02d:%02d:%02d", TempValue[0], TempValue[1], TempValue[2]));
			break;
		case MENU_DEF_DATE:
			YIELDEXEC(lcd->Writef("\n  %04d/%02d/%02d", TempValue[0], TempValue[1], TempValue[2]));
			break;
	}
}

void Menu::RenderMenuSelection() {
	//
	// Clean up last selection.
	YIELDEXEC(lcd->Goto(MenuLastSelection + 2, 1));
	YIELDEXEC(lcd->Write(" "));

	//
	// Print current selection.
	YIELDEXEC(lcd->Goto(MenuSelection + 2, 1));
	YIELDEXEC(lcd->Write(">"));
}
