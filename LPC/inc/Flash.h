/*
 * Flash.h
 *
 *  Created on: 10/06/2015
 *      Author: sergio.ferreira
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <common.h>



#define IAP_LOCATION 0x1FFF1FF1

typedef void (*IAP)(uint32_t [], uint32_t[]);


class Flash {
public:
	Flash(uint32_t startSector,	uint32_t endSector);

	uint32_t WriteData(void * dstAddr, void *srcAddr, uint32_t size);
private:
	uint32_t WriteBlock( void *dstAddr, void *srcAddr, uint32_t size);
	uint32_t PrepareForWrite();
	uint32_t EraseSectors();
	uint32_t startSector;
	uint32_t endSector;
	IAP iap_entry;
	uint32_t command[5];
	uint32_t output[5];
};

#endif /* FLASH_H_ */
