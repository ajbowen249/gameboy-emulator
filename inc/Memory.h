#ifndef __Memory_h__
#define __Memory_h__

#include <cstdint>
#include <memory>

class Memory {
public:
    typedef std::shared_ptr<Memory> Ptr;

    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;
};

#endif // __Memory_h__
