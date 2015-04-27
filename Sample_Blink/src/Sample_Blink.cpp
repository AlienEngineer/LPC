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
#include "GPIO.h"
#include "Timer.h"

#define LED_PIN   22

int main(void) {
    GPIO gpio0 (GPIO0);
    Timer timer;
    
    gpio0.PinMode(LED_PIN, OUTPUT);
    timer.Init();
    
    while(1) {
     
        gpio0.DigitalWrite(HIGH);
        timer.DelayMS(500);
        
        gpio0.DigitalWrite(LOW);
        timer.DelayMS(500);
    }
    return 0 ;
}
