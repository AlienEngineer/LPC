#include <Ethernet.h>
#include <PIN.h>
#include <RMII.h>
#include <Timers.h>
#include <string.h>
#include <stdio.h>

#define PINSEL_ETHERNET     1
#define PCOMP_ETHERNET     (1<<30)

#define EMAC_FULLDUPLEX    (1 << 0)
#define CMD_FULLDUPLEX     (1 << 10)

/* REGISTER MAC1 page 159 */
#define EMAC_MAC1_RECEIVE_ENABLE	(1 << 0)
#define EMAC_MAC1_PASS_ALL_FRAMES	(1 << 1)
#define EMAC_MAC1_RX_FLOW_CONTROL	(1 << 2)
#define EMAC_MAC1_TX_FLOW_CONTROL	(1 << 3)
#define EMAC_MAC1_LOOPBACK			(1 << 4)
#define EMAC_MAC1_RESET_TX			(1 << 8)
#define EMAC_MAC1_RESET_MCS_TX		(1 << 9)
#define EMAC_MAC1_RESET_RX			(1 << 10)
#define EMAC_MAC1_RESET_MCS_RX		(1 << 11)
#define EMAC_MAC1_SIMULATION_RESET	(1 << 14)
#define EMAC_MAC1_SOFT_RESET		(1 << 15)
#define EMAC_MAC1_RESETS			(EMAC_MAC1_RESET_TX | EMAC_MAC1_RESET_MCS_TX | EMAC_MAC1_RESET_RX | EMAC_MAC1_RESET_MCS_RX | EMAC_MAC1_SIMULATION_RESET)

/* REGISTER MAC2 page 160 */
#define EMAC_MAC2_FULL_DUPLEX		(1 << 0)
#define EMAC_MAC2_FRAME_LEN_CHECK	(1 << 1)
#define EMAC_MAC2_HUGE_FRAME_ENABLE	(1 << 2)
#define EMAC_MAC2_DELAYED_CRC		(1 << 3)
#define EMAC_MAC2_CRC_ENABLE		(1 << 4)
#define EMAC_MAC2_PAD_CRC_ENABLE	(1 << 5)
#define EMAC_MAC2_VLAN_PAD_ENABLE	(1 << 6)
#define EMAC_MAC2_AUTO_PAD_ENABLE	(1 << 7)
#define EMAC_MAC2_PURE_PREAMBLE		(1 << 8)
#define EMAC_MAC2_LONG_PREAMBLE		(1 << 9)
#define EMAC_MAC2_NO_BACKOFF		(1 << 12)
#define EMAC_MAC2_BACK_PRESSURE		(1 << 13)
#define EMAC_MAC2_EXCESS_DEFER		(1 << 14)

/* REGISTER Command page 167 */
#define EMAC_CMD_RX_ENABLE			(1 << 0)
#define EMAC_CMD_TX_ENABLE			(1 << 1)
#define EMAC_CMD_REG_RESET			(1 << 3)
#define EMAC_CMD_TX_RESET			(1 << 4)
#define EMAC_CMD_RX_RESET			(1 << 5)
#define EMAC_CMD_PASS_RUNT_FRAME	(1 << 6)
#define EMAC_CMD_PASS_RX_FILTER		(1 << 7)
#define EMAC_CMD_TX_FLOW_CONTROL	(1 << 8)
#define EMAC_CMD_RMII				(1 << 9)
#define EMAC_CMD_FULLDUPLEX			(1 << 10)
#define EMAC_CMD_RESETS				(EMAC_CMD_REG_RESET | EMAC_CMD_RX_RESET | EMAC_CMD_TX_RESET)

/* REGISTER MCFG page 163 */
#define EMAC_MCFG_MII_RESET			(1 << 15)
#define EMAC_MCFG_SPEED_DIVIDER		(0xF << 2)  // divide clock by 64 see manual for other speeds

/* REGISTER IPGT page 161 */
#define EMAC_IPGT_HALFDUPLEX		(0x12)
#define EMAC_IPGT_FULLDUPLEX		(0x15)

#define ADDR 	0x100
bool Ethernet::initialized = false;

void RMII_Write(uint32_t value, uint32_t reg) {
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

uint32_t RMII_Read(uint32_t reg) {


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

void Ethernet::Init() {

	if (initialized) {
		return;
	}

	RMII rmii;
	Timer timer;

	PIN::SetFunction(1, 0, PINSEL_ETHERNET);
	PIN::SetFunction(1, 1, PINSEL_ETHERNET);
	PIN::SetFunction(1, 4, PINSEL_ETHERNET);
	PIN::SetFunction(1, 8, PINSEL_ETHERNET);
	PIN::SetFunction(1, 9, PINSEL_ETHERNET);
	PIN::SetFunction(1, 10, PINSEL_ETHERNET);
	PIN::SetFunction(1, 14, PINSEL_ETHERNET);
	PIN::SetFunction(1, 15, PINSEL_ETHERNET);
	PIN::SetFunction(1, 16, PINSEL_ETHERNET);
	PIN::SetFunction(1, 17, PINSEL_ETHERNET);

	// Set power setting to ethernet. Turn on Ethernet.
	LPC_SC->PCONP |= PCOMP_ETHERNET;

	LPC_EMAC->MAC1 = EMAC_MAC1_RESETS; // Set all resets!
	timer.DelayMS(100);
	LPC_EMAC->MAC1 = EMAC_MAC1_RECEIVE_ENABLE; // Set options, remove resets

	LPC_EMAC->Command |= EMAC_CMD_RESETS;

	timer.DelayMS(100);

	LPC_EMAC->MAC2 = EMAC_MAC2_CRC_ENABLE | EMAC_MAC2_PAD_CRC_ENABLE;
	LPC_EMAC->IPGR = 0x12;

	LPC_EMAC->MCFG = EMAC_MCFG_SPEED_DIVIDER | EMAC_MCFG_MII_RESET;
	timer.DelayMS(100);
	LPC_EMAC->MCFG = EMAC_MCFG_SPEED_DIVIDER;

	LPC_EMAC->Command = EMAC_CMD_PASS_RX_FILTER | EMAC_CMD_TX_FLOW_CONTROL | EMAC_CMD_PASS_RUNT_FRAME | EMAC_CMD_RMII;
	timer.DelayMS(100);

	// Reset RMII
	rmii.Reset();
	timer.DelayMS(100);

	// printf("RMII id %d\n", rmii.GetId());

	uint32_t flags = rmii.Autonegotiate();

	if (flags & RMII_FULLDUPLEX) {
		LPC_EMAC->MAC2 |= EMAC_FULLDUPLEX; // Full duplex
		LPC_EMAC->Command |= CMD_FULLDUPLEX;
		LPC_EMAC->IPGT = EMAC_IPGT_FULLDUPLEX;
	}

	if (flags & RMII_SPEED100) {
		LPC_EMAC->SUPP = (1 << 8);
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

	for (uint32_t i = 0; i < NUM_RX_FRAG; ++i) {
		((EthernetDescriptor*) LPC_EMAC->RxDescriptor)[i].frame = &((EthernetFrame *) (RX_BUF_BASE))[i];
		((EthernetDescriptor*) LPC_EMAC->RxDescriptor)[i].control = ((1<<31) | (ETH_FRAG_SIZE-1));
	}

	for (uint32_t i = 0; i < NUM_TX_FRAG; ++i) {
		((EthernetDescriptor*) LPC_EMAC->TxDescriptor)[i].frame = &((EthernetFrame *) (TX_BUF_BASE))[i];
		((EthernetDescriptor*) LPC_EMAC->TxDescriptor)[i].control = ETH_FRAG_SIZE;
	}

	LPC_EMAC->RxFilterCtrl = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
	LPC_EMAC->SA0 = 0x1234;
	LPC_EMAC->SA1 = 0x1;
	LPC_EMAC->SA2 = 0x2;

	LPC_EMAC->IntEnable = (1<<3) | (1 << 7);
	/* Reset all interrupts */
	LPC_EMAC->IntClear  = 0xFFFF;

	// Enable EMAC
	LPC_EMAC->Command |= EMAC_CMD_RX_ENABLE | EMAC_CMD_TX_ENABLE;
	LPC_EMAC->MAC1 |= EMAC_MAC1_RECEIVE_ENABLE;

	timer.DelayMS(100);

	initialized = true;
}

uint32_t Ethernet::Send(void * data, uint32_t size) {

	uint32_t index = LPC_EMAC->TxProduceIndex;
	uint32_t indexNext = index+1;

	if (indexNext > LPC_EMAC->TxDescriptorNumber) {
		indexNext = 0;
	}

	if (indexNext == LPC_EMAC->TxConsumeIndex) {
		return FULL;
	}

	//printf("sending %d index: %d\n", size, LPC_EMAC->TxProduceIndex);


	EthernetDescriptor * descriptor = &((EthernetDescriptor *) LPC_EMAC->TxDescriptor)[index];

	// Copy data to the TxBuffer to send.
	memcpy(&(descriptor->frame->dstAddr[0]), data, size);

	descriptor->control = ((1<<28) | (1<<29) |(1<<30)|(1<<31) | (1<<26) | (size-1) & 0x7ff);

	// Tell hardware to dispatch data
	LPC_EMAC->TxProduceIndex = indexNext;

	return size;
}

uint32_t Ethernet::Receive(void * buffer, uint32_t bufferSize) {
	if (LPC_EMAC->RxProduceIndex != LPC_EMAC->RxConsumeIndex) {
		EthernetDescriptor * data = &((EthernetDescriptor *) LPC_EMAC->RxDescriptor)[LPC_EMAC->RxConsumeIndex];
		EthernetStatus * status = &((EthernetStatus *) LPC_EMAC->RxStatus)[LPC_EMAC->RxConsumeIndex];

		memcpy(buffer, &(data->frame->dstAddr[0]), bufferSize);

		// Inform hardware that we have collected data.
		LPC_EMAC->RxConsumeIndex++;

		// Wrap to zero.
		if (LPC_EMAC->RxConsumeIndex == NUM_RX_FRAG) {
			LPC_EMAC->RxConsumeIndex = 0;
		}

		//printf("received %d bytes\n", status->info & 0x7FF);

		// Return the received size, first 10 bits.
		return status->info & 0x7FF;
	}

	return EMPTY;
}
