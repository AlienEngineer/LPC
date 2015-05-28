#include <RMII.h>

#define ADDR 	0x100

#define REGISTER0       0
#define REGISTER1       1
#define REGISTER2       2

#define RESET           (1<<15)
#define AUTONEGOTIATE   (1<<12)
#define COMPLETE        (1<<5)

#define __SPEED100        (1<<10 | 1<<12)
#define __FULLDUPLEX      (1<<13 | 1<<11)

RMII::RMII() 
{
    this->reseted = FALSE;
}

void RMII::Write(uint32_t value, uint32_t reg) 
{
    // 1) Write 0 to MCMD
	LPC_EMAC->MCMD = 0;

	// 2) Write PHY address and register address to MADR
	LPC_EMAC->MADR = (ADDR | reg);

	// 3) Write data to MWTD
	LPC_EMAC->MWTD = (value & 0xFFFF);

	// 4) Wait for busy bit to be cleared in MIND
	while(1) {
		if ((LPC_EMAC->MIND & BUSY) == 0) break;
	}
}

uint32_t RMII::Read(uint32_t reg)
{
    // 1) Write 1 to MCMD
	LPC_EMAC->MCMD = 1;

	// 2) Write PHY address and register address to MADR
	LPC_EMAC->MADR = (ADDR | reg);

	// 3) Wait for busy bit to be cleared in MIND
	while(1) {
		if ((LPC_EMAC->MIND & BUSY) == 0) break;
	}

	// 4) Write 0 to MCMD
	LPC_EMAC->MCMD = 0;

	// 5) Read data from MRDD
	return (uint32_t)LPC_EMAC->MRDD;
}

void RMII::Reset() 
{
    this->Write(RESET, REGISTER0);

	// Wait for reset completes
	while((this->Read(REGISTER0) & RESET) == TRUE) {	}

	this->reseted = TRUE;
}

uint32_t RMII::GetId() 
{
    if (this->reseted == FALSE) 
    {
        this->Reset();
    }
    
    return this->Read(REGISTER2);
}

uint32_t RMII::Autonegotiate() 
{
    uint32_t flags = 0x0;
    
    // Request negotiation.
    this->Write(AUTONEGOTIATE, REGISTER0);
    
    // Wait for auto negotiation to complete.
    uint32_t res = this->Read(REGISTER1);

    while ((res & COMPLETE) == FALSE) {
    	res = this->Read(REGISTER1);
    }
    
    // 1 - 100Mb
    // 0 - 10Mb
    if (res & __SPEED100) {
    	flags |= RMII_SPEED100;
    }
    
    // 1 - FullDuplex
    // 0 - HalfDuplex
    if (res & __FULLDUPLEX) {
        flags |= RMII_FULLDUPLEX;
    }
    
    this->Write(flags, REGISTER1);
    
    return flags;
}
