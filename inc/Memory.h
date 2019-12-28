#ifndef __Memory_h__
#define __Memory_h__

#include <cstdint>
#include <memory>

class Memory {
public:
    typedef std::shared_ptr<Memory> Ptr;

    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;

    uint16_t readLI(uint16_t addr);
    void writeLI(uint16_t addr, uint16_t value);
};

#endif // __Memory_h__
