
#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <common.h>

class Ethernet {
public:
	Ethernet();
    void Send(uint8_t * data, uint32_t size);
    void Receive(uint8_t * data, uint32_t size);
private:
    Timer * timer;
    void Reset();
    void Enable();
};

#endif /* ETHERNET_H_ */