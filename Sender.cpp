#include "Sender.h"

#include <iostream>
#include <sstream>

#include <b15f/b15f.h>

#include "HelperFunctions.h"

Sender::Sender(B15F& drv, Device device, std::string message):
    drv(drv),
    device(device),
    msg(decomposeMessage(message, device)),
    checksum(computeChecksum((uint8_t const*)message.data(), message.size())),
    decomposedChecksum(decompose(checksum, device))
{
    setDDR(drv, device);
}

void Sender::send() {
    switch (state) {
    case State::SendMarker:
        /// Um den Beginn einer Nachricht anzuzeigen, senden wir 3 aufeinander
        /// folgende Nullen in der Indikatorposition.
        endTransmission(drv);
        ++index;
        if (index == 3) {
            index = 0;
            state = State::SendMessage;
        }
        break;
    
    case State::SendMessage:
        /// Hier senden wir 3 Einheiten mit gesetzem Indikatorbit und eine
        /// Null im Anschluss, um das Ende einer Einheit anzuzeigen.
        if (index % 4 < 3) {
            transmit(drv, msg[msgIndex++], device);
        }
        else {
            endTransmission(drv);
        }
        ++index;
        if (msgIndex == msg.size()) {
            msgIndex = index = 0;
            state = State::SendChecksum;
        }
        break;
        
    case State::SendChecksum:
        transmit(drv, decomposedChecksum[index++], device);
        if (index == 3) {
            index = 0;
            state = State::SendMarker;
        }
        break;
    }
}
