/*
 * Thermometer.cpp
 *
 *  Created on: 03/05/2015
 *      Author: Alien
 */

#include <Thermometer.h>


//  DS1631/DS1631A/DS1731
//  High-Precision Digital
//  Thermometer and Thermostat

#define DS1631_ADDR					((uint8_t)0x92)
#define DS1631CMD_START_CONV		((uint8_t)0x51)
#define DS1631CMD_STOP_CONV			((uint8_t)0x22)
#define DS1631CMD_READ_TEMP			((uint8_t)0xAA)
#define DS1631CMD_ACCESS_TH			((uint8_t)0xA1)
#define DS1631CMD_ACESS_TL			((uint8_t)0xA2)
#define DS1631CMD_ACESS_CONF		((uint8_t)0xAC)
#define DS1631CMD_SOFTW_POR			((uint8_t)0x54)

#define READ						((uint8_t)0x00000001)
#define WRITE						((uint8_t)0x00000000)

 /* FrequÃªncia usada na transmissÃ£o e na recepÃ§Ã£o de dados. */
#define FREQUENCY 					(uint32_t)200000

#define ERROR						((int8_t)(125))

Thermometer::Thermometer() {
	this->i2c.Init(LPC_I2C1, FREQUENCY);

	char buffer[2];

	buffer[0] = DS1631CMD_ACESS_CONF;
	buffer[1] = 0x0C;

	i2c.Transfer(DS1631_ADDR, WRITE, buffer, 2);
}

int8_t Thermometer::GetTemperature() {
	uint32_t status;
	char buffer[2];

	/* Inicia processo de conversÃ£o no sensor de temperatura. */
	buffer[0] = DS1631CMD_START_CONV;
	status = i2c.Transfer(DS1631_ADDR, WRITE, buffer, 1);
	if (status != 0x28)
		return ERROR;

	/* Leitura de temperatura. */
	buffer[0] = DS1631CMD_READ_TEMP;

	status = i2c.Transfer(DS1631_ADDR, WRITE, buffer, 1); /* Comando de leitura de temperatura. */
	if (status != 0x28)
		return ERROR;

	status = i2c.Transfer(DS1631_ADDR, READ, buffer, 2); /* Leitura da temperatura */
	if (status != 0xf8)
		return ERROR;

	return (int8_t) buffer[0];
}
