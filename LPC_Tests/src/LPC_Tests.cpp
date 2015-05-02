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
#include <Timer.h>

#define LED_PIN   22

#define TIMER_TEST


void delay(uint32_t millis) {
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
void HeartBeat_ST(GPIO gpio, Timer timer) {
	/*                         __    __
	 * wave form        ______/  \__/  \______
	 */

	gpio.DigitalWrite(LED_PIN, LOW);
	timer.DelayMS(500);

	gpio.DigitalWrite(LED_PIN, HIGH);
	timer.DelayMS(100);
	gpio.DigitalWrite(LED_PIN, LOW);
	timer.DelayMS(100);
	gpio.DigitalWrite(LED_PIN, HIGH);
	timer.DelayMS(100);
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
	PIN pin(1, 16); // shoud point to LPC_PINCON->PINSEL3

	// Remove GPIO function for led pin.
	led.SetFunction(2);
	pin.SetFunction(2);

	TestGPIO();
}

void TestTIMER() {
	GPIO gpio0(LPC_GPIO0);
	Timer timer(SYSTICK);

	gpio0.PinMode(LED_PIN, OUTPUT);

	while (1) {
		HeartBeat_ST(gpio0, timer);
	}
}

int main(void) {

#ifdef PINSEL_TEST
	TestPINSEL();
#endif

#ifdef TIMER_TEST
	TestTIMER();
#endif

	// By default test GPIO with heart beat
	while(1) {

		TestGPIO();

	}

	return 0;
}
