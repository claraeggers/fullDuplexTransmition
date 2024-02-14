#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

class B15F;

enum class Device {
    // Device::A sendet in den unteren 4 Bits und liest die oberen 4 Bits.
    A,
    
    // Device::B sendet in den oberen 4 Bits und liest die unteren 4 Bits.
    B,
};

/// ???
void setDDR(B15F& drv, Device device);

/// Übertrage je nach Gerät die unteren oder oberen 4 Bits von data
/// Im 0. bzw. 4. Bit (0-indiziert) wird eine 1 übertragen, um eine laufende
/// Übertragung anzuzeigen.
void transmit(B15F& drv, uint8_t data, Device device);

/// Sende Nullen in allen Bits um das Ende einer Übertragung anzuzeigen.
void endTransmission(B15F& drv);

/// Lese Pin A.
/// Der Rückgabewert zeigt an, ob eine Übertragung durch den Sender
/// beabsichtigt war, d.h. ob im 0. bzw. im 4. Bit eine 1 übertragen wurde.
/// Bei beabsichtigter Übertragung wird das Ergebis in die Variable result
/// geschrieben.
bool readPinA(B15F& drv, uint8_t& result, Device device);


#endif // DEVICE_H
