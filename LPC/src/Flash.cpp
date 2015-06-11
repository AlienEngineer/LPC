/*
 * Flash.cpp
 *
 *  Created on: 10/06/2015
 *      Author: sergio.ferreira
 */

#include <Flash.h>

#define PREPAIR_SECTOR_COMMAND		50
#define COPY_RAM_TO_FLASH_COMMAND	51
#define ERASE_SECTOR_COMMAND		52

#define CMD_SUCESS  								0
#define INVALID_COMMAND 							1
#define SRC_ADDR_ERROR  							2
#define DST_ADDR_ERROR  							3
#define SRC_ADDR_NOT_MAPPED 						4
#define DST_ADDR_NOT_MAPPED 						5
#define COUNT_ERROR 								6
#define INVALID_SECTOR  							7
#define SECTOR_NOT_BLANK    						8
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 	9
#define COMPARE_ERROR   							10

#define SECTORSIZE 									4096

Flash::Flash(uint32_t startSector, uint32_t endSector) {
	iap_entry = (IAP) IAP_LOCATION;
	this->startSector = startSector;
	this->endSector = endSector;
}

uint32_t Flash::WriteBlock(void *dstAddr, void *srcAddr, uint32_t size) {
	if (this->PrepareForWrite() == CMD_SUCESS) {
		this->command[0] = COPY_RAM_TO_FLASH_COMMAND;
		this->command[1] = (uint32_t)dstAddr;
		this->command[2] = (uint32_t)srcAddr;
		this->command[3] = size;
		// this->command[4] = CLK / 1000;

		iap_entry(this->command, this->output);
	}

	return this->output[0];
}
uint32_t Flash::PrepareForWrite() {
	this->command[0] = PREPAIR_SECTOR_COMMAND;
	this->command[1] = this->startSector;
	this->command[2] = this->endSector;

	iap_entry(this->command, this->output);

	return this->output[0];
}

uint32_t Flash::WriteData(void * dstAddr, void *srcAddr, uint32_t size) {
	uint32_t result;

	if ((result = this->EraseSectors()) == CMD_SUCESS) {
		result = this->WriteBlock(dstAddr, srcAddr, size);

		if (result == CMD_SUCESS && size > SECTORSIZE) {
			result = this->WriteBlock(dstAddr + SECTORSIZE, srcAddr + SECTORSIZE, size);
		}
	}

	return result;
}

uint32_t Flash::EraseSectors() {
	if (this->PrepareForWrite() == CMD_SUCESS) {
		this->command[0] = ERASE_SECTOR_COMMAND;
		this->command[1] = startSector;
		this->command[2] = endSector;
		// this->command[3] = CLK / 1000;
		iap_entry(this->command, this->output);
	}

	return this->output[0];
}

///* Apaga o conteúdo de um sector, ou de múltiplos sectores, da FLASH. Para apagar
// apenas um sector, deve usar-se o mesmo número de sector para os dois parâmetros. */
//unsigned int FLASH_EraseSectors(unsigned int startSector,
//		unsigned int endSector) {
//	if (prepareForWrite(startSector, endSector) == CMD_SUCESS) {
//		iap_command[0] = 52;/*Erasesectorscommand*/
//		iap_command[1] = startSector;
//		iap_command[2] = endSector;
//		iap_command[3] = CLK / 1000;
//		iap_entry(iap_command, iap_result);
//	}
//	return iap_result[0];
//}
//
///* Escreve o bloco de dados referenciado por srcAddr, de dimensão size bytes, no
// endereço da FLASH referenciado por dstAddr. Pressupõe que os sectores envolvidos na
// operação de escrita foram apagados previamente e que size não excede 4kB. */
//unsigned int FLASH_WriteBlock(void *dstAddr, void *srcAddr, unsigned int size) {
//	if (prepareForWrite(14, 14) == CMD_SUCESS) {
//		iap_command[0] = 51;/*Prepairsectorscommand*/
//		iap_command[1] = dstAddr;
//		iap_command[2] = srcAddr;
//		iap_command[3] = size;
//		iap_command[4] = CLK / 1000;
//		iap_entry(iap_command, iap_result);
//	}
//	return iap_result[0];
//}
//
///* Escreve o bloco de dados referenciado por srcAddr, de dimensão size bytes, no
// endereço da FLASH referenciado por dstAddr. */
//
//unsigned int FLASH_WriteData(void * dstAddr, void *srcAddr, unsigned int size) {
//	int result;
//
//	if ((result = FLASH_EraseSectors(14, 14)) == CMD_SUCESS) {
//		result = FLASH_WriteBlock(dstAddr, srcAddr, size);
//
//		if (result == CMD_SUCESS && size > SECTORSIZE / 2) {
//			result = FLASH_WriteBlock(dstAddr + SECTORSIZE / 2,
//					srcAddr + SECTORSIZE / 2, size);
//		}
//	}
//
//	return result;
//}
//
///* Compara o conteúdo do bloco de dados referenciado por srcAddr, de dimensão size
// bytes, com o conteúdo do bloco de dados referenciado por dstAddr. */
//unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size) {
//	unsigned int iap_command[5], iap_result[3];
//	iap_command[0] = 56;/*Prepairsectorscommand*/
//	iap_command[1] = dstAddr;
//	iap_command[2] = srcAddr;
//	iap_command[3] = size;
//	iap_entry(iap_command, iap_result);
//	return iap_result[0];
//}
