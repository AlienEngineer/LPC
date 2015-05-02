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
#include <PIN.h>

#define LED_PIN   22



void delay(int millis) {
	millis *= SystemCoreClock / 1000;
	millis /= 4;
	while (millis) {
		--millis;
	}
}

void HeartBeat(GPIO gpio) {
	/*                         __    __
	 * wave form        ______/  \__/  \______
	 */

	gpio.DigitalWrite(LED_PIN, LOW);
	delay(500);

	gpio.DigitalWrite(LED_PIN, HIGH);
	delay(100);
	gpio.DigitalWrite(LED_PIN, LOW);
	delay(100);
	gpio.DigitalWrite(LED_PIN, HIGH);
	delay(100);
}

void TestGPIO() {
	GPIO gpio0(LPC_GPIO0);
	gpio0.PinMode(LED_PIN, OUTPUT);

	while (1) {
		HeartBeat(gpio0);
	}
}

void TestPINSEL() {
	PIN led(0, LED_PIN);

	// Remove GPIO function for led pin.
	led.SetFunction(2);


	TestGPIO();
}

int main(void) {

#ifdef GPIO_TEST
	TestGPIO();
#endif

#ifdef PINSEL_TEST
	TestPINSEL();
#endif

	// By default test GPIO with heart beat
	while(1) {

		TestGPIO();

	}

	return 0;
}
