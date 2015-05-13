#ifndef RMII_H_
#define RMII_H_

#include <common.h>

#define RMII_SPEED100        (1 << 13)
#define RMII_FULLDUPLEX      (1 << 8)

class RMII {
public:
    RMII();
	void Write(uint32_t value, uint32_t reg);
    uint32_t Read(uint32_t reg);
    void Reset();
    uint32_t GetId();
    uint32_t Autonegotiate();
    
private:
    uint8_t reseted;
};

#endif /* RMII_H_ */