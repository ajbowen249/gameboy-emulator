#ifndef __SimpleMemory_h__
#define __SimpleMemory_h__

#include <vector>

#include "Memory.h"

class SimpleMemory : public Memory {
public:
    SimpleMemory();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;
    void write(uint16_t startAddr, const std::vector<uint8_t>& values);

    std::vector<uint8_t> _mainMemory;
};

#endif // __SimpleMemory_h__
