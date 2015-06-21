/*
 * Menu.h
 *
 *  Created on: 20/06/2015
 *      Author: sergio.ferreira
 */

#ifndef MENU_H_
#define MENU_H_

#include <LCD.h>


#define MENU_NONE		0
#define MENU_PRESENT	1
#define MENU_DEF_TMAX	2
#define MENU_DEF_TMIN	3
#define MENU_DEF_CLOCK	4
#define MENU_DEF_DATE	5
#define MENU_SHOW_TEMP	6

#define DEF_TMAX		0
#define DEF_TMIN		1
#define DEF_CLOCK		2
#define DEF_DATE		3
#define SHOW_TEMP		4
#define EXIT			5

#define LOG_VIEW_PAGE_SIZE 10

#define MENU_MIN		0
#define MENU_MAX		5

class Menu {
public:
	Menu(LCD * lcd);
	void SwitchMenuToSelection(uint8_t index);
	void Render();
	void MoveTo(uint8_t state);
	void GoUp();
	void GoDown();
	uint8_t GetMenuState();
	void Enter();
private:
	void RenderMenuSelection();
	void RenderTempValue();
	void HandleSelection();
	void HandleTempValueEnter();
	LCD * lcd;
	uint8_t MenuState;
	uint8_t MenuSelection;
	uint8_t MenuLastSelection;
	int32_t TempValue[3];
	int32_t TempIndex;
};


#endif /* MENU_H_ */

