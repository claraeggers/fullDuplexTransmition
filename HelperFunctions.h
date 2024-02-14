#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <array>
#include <string>
#include <vector>
#include <stdint.h>

#include "Device.h"

/// Zerlege ein Byte in jeweils 3 Bytes in denen nur die oberen bzw. die
/// unteren 3 Bits belegt sind und setze sie wieder zusammen, basierend auf dem
/// verwendeten Gerät.
/// D.h. die resultieren Bytes sind maskiert durch 0000'1110 bzw. 1110'0000.
std::array<uint8_t, 3> decompose(uint8_t value, Device device);
uint8_t compose(std::array<uint8_t, 3> parts, Device device);

/// Die folgenden Funktionen sind Implementierungsdetails und nur zu
/// Testzwecken hier im Header deklariert.
std::array<uint8_t, 3> decomposeToLowerBits(uint8_t value);
uint8_t composeFromLowerBits(std::array<uint8_t, 3> parts);
std::array<uint8_t, 3> decomposeToUpperBits(uint8_t value);
uint8_t composeFromUpperBits(std::array<uint8_t, 3> parts);

/// Zerlege jedes Byte einer Nachricht mithilfe der Funktion \p decompose und
/// gebe die gesamte zerlegte Nachricht zurück.
std::vector<uint8_t> decomposeMessage(std::string const& message, Device device);

/// Füge eine zerlegte Nachricht wieder zusammen. 
std::string composeMessage(uint8_t const* data, size_t size, Device device);

/// Berechne eine Prüfsumme aus einem Datensatz.
uint8_t computeChecksum(uint8_t const* data, size_t size);

std::string readUserInput();

#endif // HELPERFUNCTIONS_H
