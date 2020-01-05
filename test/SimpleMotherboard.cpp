#include "SimpleMotherboard.h"

SimpleMotherboard::SimpleMotherboard() :_mainMemory(65536) { }

uint8_t SimpleMotherboard::read(uint16_t addr) {
    return _mainMemory[(size_t)addr];
}

void SimpleMotherboard::write(uint16_t addr, uint8_t value) {
    _mainMemory[(size_t)addr] = value;
}

void SimpleMotherboard::write(uint16_t startAddr, const std::vector<uint8_t>& values) {
    size_t address = startAddr;
    for (const auto value : values) {
        _mainMemory[(size_t)address++] = value;
    }
}
