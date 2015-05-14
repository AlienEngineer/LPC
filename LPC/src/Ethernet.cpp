
#include <Ethernet.h>
#include <PIN.h>
#include <RMII.h>
#include <TIMER.h>

#define PINSEL_ETHERNET     1
#define PCOMP_ETHERNET     (1<<30)

#define EMAC_FULLDUPLEX    (1 << 0)
#define CMD_FULLDUPLEX     (1 << 10)

/* EMAC Memory Buffer configuration for 16K Ethernet RAM. */
#define NUM_RX_FRAG 		4 /* Num.of RX Fragments 4*1536= 6.0kB */
#define NUM_TX_FRAG 		2 /* Num.of TX Fragments 3*1536= 4.6kB */
#define ETH_FRAG_SIZE 		1536 /* Packet Fragment size 1536 Bytes */
#define ETH_MAX_FLEN 		1536 /* Max. Ethernet Frame Size */

/* EMAC variables located in AHB SRAM bank 1 */
#define RX_DESC_BASE 		0x2007c000
#define RX_STAT_BASE 		(RX_DESC_BASE + NUM_RX_FRAG*8)
#define TX_DESC_BASE 		(RX_STAT_BASE + NUM_RX_FRAG*8)
#define TX_STAT_BASE 		(TX_DESC_BASE + NUM_TX_FRAG*8)
#define RX_BUF_BASE 		(TX_STAT_BASE + NUM_TX_FRAG*4)
#define TX_BUF_BASE 		(RX_BUF_BASE + NUM_RX_FRAG*ETH_FRAG_SIZE)

Ethernet::Ethernet(Timer * timer) {
    RMII rmii;
    
    this->timer = timer;
    
    PINSEL(2, 0, PINSEL_ETHERNET);
    PINSEL(2, 1, PINSEL_ETHERNET);
    PINSEL(2, 4, PINSEL_ETHERNET);
    PINSEL(2, 8, PINSEL_ETHERNET);
    PINSEL(2, 9, PINSEL_ETHERNET);
    PINSEL(2, 10, PINSEL_ETHERNET);
    PINSEL(2, 14, PINSEL_ETHERNET);
    PINSEL(2, 15, PINSEL_ETHERNET);
    
    PINSEL(3, 16, PINSEL_ETHERNET);
    PINSEL(3, 17, PINSEL_ETHERNET);
    
    // Set power setting to ethernet. Turn on Ethernet.
    LPC_SC->PCONP |= PCOMP_ETHERNET;
    
    LPC_EMAC->MAC1 = (1 << 1);
    LPC_EMAC->MCFG = (0xF << 2); // divide clock by 64
    LPC_EMAC->MAC2 = (1<<5) | (1<<4);
	LPC_EMAC->IPGR = 0x12;
    LPC_EMAC->Command = (1 << 9) | (1 << 7) | (1 << 6);
    LPC_EMAC->RxFilterCtrl = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4);
    
    // Reset EMAC
    this->Reset();
    
	// Reset RMII
    rmii.Reset();
    uint32_t flags = rmii.Autonegotiate();
    
    if (flags & RMII_FULLDUPLEX)
    {
        LPC_EMAC->MAC2 |= EMAC_FULLDUPLEX; // Full duplex
        LPC_EMAC->Command |= CMD_FULLDUPLEX;
    }
    
    // RECEIVE
    LPC_EMAC->RxConsumeIndex = 0;
    LPC_EMAC->RxDescriptor = RX_DESC_BASE;
    LPC_EMAC->RxDescriptorNumber = NUM_RX_FRAG - 1;
    LPC_EMAC->RxStatus = RX_STAT_BASE;

    // SEND
    LPC_EMAC->TxProduceIndex = 0;
    LPC_EMAC->TxDescriptor = TX_DESC_BASE;
    LPC_EMAC->TxDescriptorNumber = NUM_TX_FRAG - 1;
    LPC_EMAC->TxStatus = TX_STAT_BASE;
    
    LPC_EMAC->SA0 = 0x1234;
    LPC_EMAC->SA1 = 0x1;
    LPC_EMAC->SA2 = 0x2;
    
    // Enable EMAC
    this->Enable();
}

void Ethernet::Enable() 
{
    LPC_EMAC->Command |= (1<<0) | (1<<1);
    LPC_EMAC->MAC1 |= 1;

    this->timer->DelayMS(100);
}

void Ethernet::Reset() 
{
    uint32_t mac1 = LPC_EMAC->MAC1;
    uint32_t cmd = LPC_EMAC->Command;
    uint32_t mcfg = LPC_EMAC->MCFG;
    
    // Set all resets!
    LPC_EMAC->MAC1 = (1<<8) | (1<<9) | (1<<10) | (1<<11) | (1<<14);
    LPC_EMAC->Command = (1 << 3) | (1 << 4) | (1 << 5);
    LPC_EMAC->MCFG = (0xF << 2) | (1<<15);
	
    this->timer->DelayMS(20);
    
    // Clear all resets!
    LPC_EMAC->MAC1 = mac1 & ~LPC_EMAC->MAC1;
	LPC_EMAC->MCFG = mcfg & ~LPC_EMAC->MCFG;
    LPC_EMAC->Command = cmd & ~LPC_EMAC->Command;
    
    this->timer->DelayMS(100);
}

void Ethernet::Send(void * data, uint32_t size)
{
    
}

void Ethernet::Receive(void * buffer, uint32_t bufferSize)
{
    
}
