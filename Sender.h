#ifndef SENDER_H
#define SENDER_H

#include <array>
#include <string>
#include <vector>

#include "Device.h"

class B15F;

class Sender {
public:
    explicit Sender(B15F& drv, Device device, std::string message);
    
    void send();
    
    enum class State {
        SendMarker,
        SendMessage,
        SendChecksum,
    };
    
private:
    B15F& drv;
    Device device;
    std::vector<uint8_t> msg;
    uint8_t checksum;
    std::array<uint8_t, 3> decomposedChecksum;
    State state = State::SendMarker;
    size_t index = 0, msgIndex = 0;
};

#endif // SENDER_H
