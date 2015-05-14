
#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <common.h>
#include <Timer.h>

typedef struct {
    uint32_t info;
    uint32_t hash;
} EthernetStatus;

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
