#include "HelperFunctions.h"


#include <algorithm>
#include <iostream>

std::array<uint8_t, 3> decomposeToLowerBits(uint8_t x) {
    return {
        uint8_t((x << 1) & 0xE),
        uint8_t((x >> 2) & 0xE),
        uint8_t((x >> 5) & 0xE)
    };
}

uint8_t composeFromLowerBits(std::array<uint8_t, 3> parts) {
    /// Maskiere alle Bytes mit 0000'1110.
    std::transform(parts.begin(), parts.end(), parts.begin(), [](uint8_t x) { return x & 0xE; });
    return (parts[0] >> 1) | (parts[1] << 2) | (parts[2] << 5);
}

std::array<uint8_t, 3> decomposeToUpperBits(uint8_t x) {
    return {
        uint8_t( x       & 0xE0),
        uint8_t((x << 3) & 0xE0),
        uint8_t((x << 6) & 0xE0)
    };
}

uint8_t composeFromUpperBits(std::array<uint8_t, 3> parts) {
    /// Maskiere alle Bytes mit 1110'0000.
    std::transform(parts.begin(), parts.end(), parts.begin(), [](uint8_t x) { return x & 0xE0; });
    return parts[0] | (parts[1] >> 3) | (parts[2] >> 6);
}

std::array<uint8_t, 3> decompose(uint8_t value, Device device) {
    switch (device) {
    case Device::A:
        return decomposeToLowerBits(value);
    case Device::B:
        return decomposeToUpperBits(value);
    }
}

uint8_t compose(std::array<uint8_t, 3> parts, Device device) {
    switch (device) {
    case Device::A:
        return composeFromUpperBits(parts);
    case Device::B:
        return composeFromLowerBits(parts);
    }
}

std::vector<uint8_t> decomposeMessage(std::string const& message, Device device) {
    std::vector<uint8_t> result;
    result.reserve(message.size() * 3);
    for (char c: message) {
        std::array<uint8_t, 3> parts = decompose(c, device);
        for (uint8_t part: parts) {
            result.push_back(part);
        }
    }
    return result;
}

std::string composeMessage(uint8_t const* data, size_t size, Device device) {
    assert(size % 3 == 0);
    std::string result;
    for (uint8_t const* const end = data + size; data != end; data += 3) {
        std::array<uint8_t, 3> parts = {
            *data,
            *(data + 1),
            *(data + 2)
        };
        result.push_back(compose(parts, device));
    }
    return result;
}

uint8_t computeChecksum(uint8_t const* data, size_t size) {
    uint8_t result = 0;
    for (uint8_t const* const end = data + size; data != end; ++data) {
        result ^= *data;
    }
    return result;
}

std::string readUserInput() {
    std::cout << "Gebe den zu übertragenden Satz ein" << std::endl;
    std::string s;
    std::getline(std::cin, s);
    return s;
}
