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
#include <LPCTimer.h>
#include <SPI.h>
#include <Ethernet.h>
#include <stdio.h>
#include <RTC.h>
#include <Flash.h>

#define LED_PIN   22

#define FLASH_TE2ST 		1

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


	TestGPIO();
}

void TestRTC() {
	GPIO gpio0(LPC_GPIO0);
	RTC rtc(2015, 6, 10, 12, 56, 0);

	gpio0.PinMode(LED_PIN, OUTPUT);

	while (1) {

		DateTime * dt = rtc.GetDate();

		if ((dt->Second%10) == 0) {
			HeartBeat(gpio0);
		}
	}
}

void TestETHERNET() {
	Ethernet::Init();

	while (1) {

	}

}

void TestFLASH() {
	Flash flash(14, 14);



	while (1) {

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

#ifdef RTC_TEST
	TestRTC();
#endif

#ifdef FLASH_TEST
	TestFLASH();
#endif

#ifdef ETHERNET_TEST
	TestETHERNET();
#endif

	PIN bt1(2, 0);
	PIN bt2(2, 1);
	PIN bt3(2, 2);

	bt1.Mode(INPUT);
	bt2.Mode(INPUT);
	bt3.Mode(INPUT);
	bt1.SetFunction(0);
	bt2.SetFunction(0);
	bt3.SetFunction(0);

	LPC_PINCON->PINSEL4 = 0x3F;

	// By default test GPIO with heart beat
	while (1) {

		// TestGPIO();

		uint32_t val = LPC_GPIO2->FIOPIN;

		bool b1 = !bt1.IsOn();
		bool b2 = !bt2.IsOn();
		bool b3 = !bt3.IsOn();

		if (b1 || b2 || b3) {
			printf("bt1: %d		bt2: %d		bt3: %d\n", b1, b2, b3);
		}

	}

	return 0;
}
