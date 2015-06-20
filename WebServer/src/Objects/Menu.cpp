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

void Menu::GoDown() {
	switch(MenuState) {
		case MENU_PRESENT:
			if (MenuSelection < MENU_MAX) {
				SwitchMenuToSelection(MenuSelection + 1);
			}
		break;

		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
		case MENU_DEF_CLOCK:
		case MENU_DEF_DATE:
			TempValue--;
			this->RenderTempValue();
		break;
	}

}

void Menu::GoUp() {
	switch(MenuState) {
		case MENU_PRESENT:
			if (MenuSelection > MENU_MIN) {
				SwitchMenuToSelection(MenuSelection - 1);
			}
			break;

		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
		case MENU_DEF_CLOCK:
		case MENU_DEF_DATE:
			TempValue++;
			this->RenderTempValue();
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
			this->HandleTempValueEnter();
			break;
	}
	this->Render();
}

void Menu::HandleTempValueEnter() {
	switch (MenuSelection) {
		case DEF_TMAX:
			context.LimitSup = TempValue;
			break;
		case DEF_TMIN:
			context.LimitInf = TempValue;
			break;
		case DEF_CLOCK:
		case DEF_DATE:
		case SHOW_TEMP:
			break;
	}

	// GO BACK
	this->MoveTo(MENU_PRESENT);
	this->SwitchMenuToSelection(0);
}

void Menu::HandleSelection() {
	switch (MenuSelection) {
		case DEF_TMAX:
			TempValue = context.LimitSup;
			this->MoveTo(MENU_DEF_TMAX);
			break;
		case DEF_TMIN:
			TempValue = context.LimitInf;
			this->MoveTo(MENU_DEF_TMIN);
			break;
		case DEF_CLOCK:
			this->MoveTo(MENU_DEF_CLOCK);
			break;
		case DEF_DATE:
			this->MoveTo(MENU_DEF_DATE);
			break;
		case SHOW_TEMP:
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

	switch (MenuState) {
		case MENU_NONE:
			YIELDEXEC(lcd->ClearScreen());
			break;
		case MENU_PRESENT:
			YIELDEXEC(lcd->ClearScreen());
			YIELDEXEC(lcd->SetCursor(10, 20));
			YIELDEXEC(lcd->Write("\n   Define Tmax"));
			YIELDEXEC(lcd->Write("\n   Define Tmin"));
			YIELDEXEC(lcd->Write("\n   Change Clock"));
			YIELDEXEC(lcd->Write("\n   Change Date"));
			YIELDEXEC(lcd->Write("\n   Temp log"));
			YIELDEXEC(lcd->Write("\n   Exit"));
			break;
		case MENU_DEF_TMAX:
			YIELDEXEC(lcd->ClearScreen());
			YIELDEXEC(lcd->Goto(1, 1));
			YIELDEXEC(lcd->Write("\n  Define Tmax:"));
			YIELDEXEC(lcd->Writef("\n  %d degrees", TempValue));
			break;
		case MENU_DEF_TMIN:
			YIELDEXEC(lcd->ClearScreen());
			YIELDEXEC(lcd->Goto(1, 1));
			YIELDEXEC(lcd->Write("\n  Define Tmin:"));
			YIELDEXEC(lcd->Writef("\n  %d degrees", TempValue));
			break;
	}
}

void Menu::RenderTempValue() {
	switch (MenuState) {
		case MENU_DEF_TMAX:
		case MENU_DEF_TMIN:
			YIELDEXEC(lcd->Goto(2, 1));
			YIELDEXEC(lcd->Writef("\n  %d degrees    ", TempValue));
			break;
	}
}

void Menu::RenderMenuSelection() {
	//
	// Clean up last selection.
	YIELDEXEC(lcd->Goto(MenuLastSelection + 3, 1));
	YIELDEXEC(lcd->Write(" "));

	//
	// Print current selection.
	YIELDEXEC(lcd->Goto(MenuSelection + 3, 1));
	YIELDEXEC(lcd->Write(">"));
}
