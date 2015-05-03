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

#include <stdio.h>
#include <LCD.h>
#include <Thermometer.h>
#include <Timer.h>

#define RESET		2
#define CS 			16

char aux_buf[20];

int main(void) {
	Thermometer thermo;
	Timer 		timer	(SYSTICK);
	PIN 		csPin	(0, CS);
	PIN 		resetPin(0, RESET);
	LCD 		lcd		(&csPin, &resetPin);



	lcd.ClearScreen();
	lcd.SetCursor(10, 20);
	lcd.Write("This is a test!");

	timer.DelayMS(2000);
	lcd.ClearScreen();

	while (1) {
		int8_t temp = thermo.GetTemperature();

		sprintf(aux_buf, "Temp:%03d", temp);

		lcd.SetCursor(10, 20);
		lcd.Write(aux_buf);

		timer.DelayMS(200);
	}

    return 0 ;
}
