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
#include <Timers.h>
#include <SPI.h>
#include <Ethernet.h>
#include <stdio.h>

#define LED_PIN   22

#define PINSEL_TEST


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

void TestSPI() {
	SPI_Config spi_config;

	spi_config.LSBF = 0;
	spi_config.SPIE = 0;
	spi_config.BITS = 9;
	spi_config.MASTER = 1;
	spi_config.CPHA = 1;
	spi_config.CPOL = 1;
	spi_config.SPCCR = 8;


	SPI spi(&spi_config);

	spi.Send(16);

	TestGPIO();
}

void TestETHERNET() {
	Timer timer;
	Ethernet ethernet(&timer);

	while(1) {


	}

}

int main(void) {

#ifdef PINSEL_TEST
	TestPINSEL();
#endif

#ifdef TIMER_TEST
	TestTIMER();
#endif

#ifdef SPI_TEST
	TestSPI();
#endif

#ifdef ETHERNET_TEST
	TestETHERNET();
#endif

	// By default test GPIO with heart beat
	while(1) {

		TestGPIO();

	}

	return 0;
}
