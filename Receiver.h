#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdint.h>
#include <array>
#include <string>

#include "Device.h"

class B15F;

class Receiver {
public:
    Receiver(B15F& drv, Device device, bool talk = true);
    void receive();
    
private:
    bool verifyMessage() const;
    
private:
    B15F& drv;
    Device device;
    std::array<uint8_t, 3> receivingChar;
    std::string receivedMessage;
    size_t index = 0;
    size_t consecZeros = 0;
    bool receivingFullMessage = false;
    bool talk;
};

#endif // RECEIVER_H
