/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#include <cr_section_macros.h>

#include <GPIO.h>

#define LED_PIN   22

void delay(int millis) {
	millis *= SystemCoreClock / 1000;
	millis /= 4;
	while (millis) {
		--millis;
	}
}

int main(void) {

	GPIO gpio0(LPC_GPIO0);

	gpio0.PinMode(LED_PIN, OUTPUT);

	while (1) {
		gpio0.DigitalWrite(LED_PIN, HIGH);
		delay(500);

		gpio0.DigitalWrite(LED_PIN, LOW);
		delay(500);
	}
	return 0;
}
