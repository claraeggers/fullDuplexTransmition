#include "Device.h"

#include <b15f/b15f.h>

void setDDR(B15F& drv, Device device) {
    switch (device) {
    case Device::A:
        drv.setRegister(&DDRA, 0x0F);
        break;
    case Device::B:
        drv.setRegister(&DDRA, 0xF0);
        break;
    }
}

void transmit(B15F& drv, uint8_t data, Device device) {
    int const indicatorBit = device == Device::A ? 0 : 4;
    data |= 1 << indicatorBit;
    drv.setRegister(&PORTA, data);
}

void endTransmission(B15F& drv) {
    drv.setRegister(&PORTA, 0);
}

bool readPinA(B15F& drv, uint8_t& result, Device device) {
    result = drv.getRegister(&PINA);
    int indicatorBit = device == Device::A ? 4 : 0;
    if (!(result & (1 << indicatorBit))) {
        result = 0;
        return false;
    }
    result &= ~(1 << indicatorBit);
    return true;
}
