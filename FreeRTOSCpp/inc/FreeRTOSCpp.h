/*
 * FreeRTOSCpp.h
 *
 *  Created on: 06/06/2015
 *      Author: sergio.ferreira
 */

#ifndef FREERTOSCPP_H_
#define FREERTOSCPP_H_

#include <LPC17xx.h>

extern "C" {
	#include <FreeRTOS.h>
	#include <task.h>
	#include <semphr.h>
}

#include <Thread.h>

#define STACK_SIZE 		512

#endif /* FREERTOSCPP_H_ */
