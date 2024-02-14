#include <assert.h>

#include "HelperFunctions.h"

void testDecompositionToLower(uint8_t value) {
    std::array<uint8_t, 3> parts = decomposeToLowerBits(value);
    for (auto p: parts) {
        assert((p & ~7) == 0);
    }
    uint8_t comp = composeFromLowerBits(parts);
    assert(comp == value);
}

void testDecompositionToUpper(uint8_t value) {
    std::array<uint8_t, 3> parts = decomposeToUpperBits(value);
    for (auto p: parts) {
        assert((p & ~0xE0) == 0);
    }
    uint8_t comp = composeFromUpperBits(parts);
    assert(comp == value);
}

void testDecomposition(uint8_t value) {
    assert(value == compose(decompose(value, Device::A), Device::B));
    assert(value == compose(decompose(value, Device::B), Device::A));
}

int main() {
    for (int i = 0; i < 0x100; ++i) {
        testDecompositionToLower(i);
        testDecompositionToUpper(i);
        testDecomposition(i);
    }
}
