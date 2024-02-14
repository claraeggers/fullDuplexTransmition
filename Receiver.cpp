#include "Receiver.h"

#include <iostream>
#include <sstream>

#include "HelperFunctions.h"

Receiver::Receiver(B15F& drv, Device device, bool talk):
    drv(drv),
    device(device),
    talk(talk) {}

constexpr char const* reset = "\u001b[0m";
constexpr char const* red   = "\u001b[31m";
constexpr char const* green = "\u001b[32m";
constexpr char const* gray  = "\u001b[37m";

void Receiver::receive() {
    uint8_t x = 0xFF;
    bool success = readPinA(drv, x, device);
    if (success) {
        consecZeros = 0;
        receivingChar[index] = x;
        ++index;
        if (index == 3) {
            index = 0;
            char const receivedChar = compose(receivingChar, device);
            receivedMessage.push_back(receivedChar);
            std::cout << receivedChar;
        }
        return;
    }
    index = 0;
    ++consecZeros;
    if (consecZeros == 3) {
        std::cout << gray << "\nWir haben 3 aufeinanderfolgende Nullen empfangen.\n" << reset;
        /// Wenn wir 3 aufeinanderfolgende Nullen in der Indikatorposition
        /// empfangen, wissen wir, dass die Übertragung einer Nachricht
        /// abgeschlossen ist und die Übertragung einer neuen Nachricht
        /// beginnt.
        verifyMessage();
        receivedMessage.clear();
        receivingFullMessage = true;
    }
}

bool Receiver::verifyMessage() const {
    if (!receivingFullMessage) {
        if (talk) {
            std::cout << "Unvollständige Nachricht empfangen.\n";
        }
        return false;
    }
    if (receivedMessage.empty()) {
        if (talk) {
            std::cout << "Empfangener Datensatz ist zu klein.\n";
        }
        return false;
    }
    uint8_t const lastByte = receivedMessage.back();
    uint8_t const checksum = computeChecksum((uint8_t const*)receivedMessage.data(), receivedMessage.size() - 1);
    if (lastByte != checksum) {
        if (talk) {
            std::cout << red << "Übertragungsfehler: Prüfsumme stimmt nicht mit den empfangenen Daten überein.\n" << reset;
            std::cout << red << "\"" << receivedMessage.substr(0, receivedMessage.size() - 1) << "\"\n" << reset;
        }
        return false;
    }
    if (talk) {
        std::cout << green << "Die Nachricht wurde erfolgreich empfangen: \n" << reset;
        std::cout << green << "\"" << receivedMessage.substr(0, receivedMessage.size() - 1) << "\"\n" << reset;
    }
    return true;
}
