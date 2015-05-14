
#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <common.h>
#include <Timer.h>

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

#define ETH_ADDR_SIZE		6

typedef struct {
    uint32_t info;
    uint32_t hash;
} EthernetStatus;

typedef struct {
	uint8_t preamble[7];
    uint8_t dstAddr[ETH_ADDR_SIZE];
    uint8_t srcAddr[ETH_ADDR_SIZE];
    uint8_t payload[ETH_FRAG_SIZE-(ETH_ADDR_SIZE*2+7)];
} EthernetFrame;


class Ethernet {
public:
	Ethernet(Timer * timer);
    void Send(void * data, uint32_t size);
    void Receive(void * buffer, uint32_t bufferSize);
private:
    Timer * timer;
    void Reset();
    void Enable();
};

#endif /* ETHERNET_H_ */
