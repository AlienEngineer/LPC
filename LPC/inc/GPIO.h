#ifndef GPIO_H_
#define GPIO_H_

#include <common.h>

/**
 * General Purpose Input Output (GPIO)
 *
 * @authro Sérgio Ferreira
 */
class GPIO {
public:
	GPIO(LPC_GPIO_TypeDef * port);
	GPIO();
    
    /**
	 * Initializes this instance of GPIO.
     *
	 * @param port a LPC_GPIO_TypeDef pointer
	 */
	void Init(LPC_GPIO_TypeDef * port);
    
	/**
	 * Set the pin mode for the given pin number.
     *
	 * @param direction
	 * @param pin
	 */
	void PinMode(uint32_t pin, uint32_t direction);

	/**
	 * Reads from the given pin. 
     *
	 * @param pin
     * @return the value read on the given pin. HIGH or LOW constant.
	 */
	uint32_t DigitalRead(uint32_t pin);

	/**
	 * Set the pin value with the given value. Use HIGH or LOW constants.
     *
	 * @param pin
	 * @param value
	 */
	void DigitalWrite(uint32_t pin, uint32_t value);

private:
	LPC_GPIO_TypeDef * GPIOPx;

};

#endif /* GPIO_H_ */
