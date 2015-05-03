/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>


#include <LCD.h>

#define LCD_RESET		2
#define CS_PIN 			16

int main(void) {
	GPIO gpio0(LPC_GPIO0);
	LCD lcd(CS_PIN, LCD_RESET, &gpio0);

	lcd.ClearScreen();
	lcd.SetCursor(10, 20);
	lcd.Write("This is a test!");

	while (1) {

	}

    return 0 ;
}
