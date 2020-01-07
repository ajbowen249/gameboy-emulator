#ifndef __Memory_h__
#define __Memory_h__

#include <cstdint>
#include <memory>

#define GROUP_LENGTH(name) (name##_END - name##_START) + 0x0001

#define ROM_BANK_0_START 0x0000
#define ROM_BANK_0_END   0x3fff
#define ROM_BANK_0_LENGTH GROUP_LENGTH(ROM_BANK_0)

#define ROM_BANK_N_START 0x4000
#define ROM_BANK_N_END   0x7fff
#define ROM_BANK_N_LENGTH GROUP_LENGTH(ROM_BANK_N)

#define TILE_RAM_START 0x8000
#define TILE_RAM_END   0x97ff
#define TILE_RAM_LENGTH GROUP_LENGTH(TILE_RAM)

#define BACKGROUND_DATA_1_START 0x9800
#define BACKGROUND_DATA_1_END   0x9bff
#define BACKGROUND_DATA_1_LENGTH GROUP_LENGTH(BACKGROUND_DATA_1)

#define BACKGROUND_DATA_2_START 0x9c00
#define BACKGROUND_DATA_2_END   0x9fff
#define BACKGROUND_DATA_2_LENGTH GROUP_LENGTH(BACKGROUND_DATA_2)

#define CART_RAM_START 0xa000
#define CART_RAM_END   0xbfff
#define CART_RAM_LENGTH GROUP_LENGTH(CART_RAM)

#define WORK_RAM_START 0xc000
#define WORK_RAM_END   0xdfff
#define WORK_RAM_LENGTH GROUP_LENGTH(WORK_RAM)

#define ECHO_RAM_START 0xe000
#define ECHO_RAM_END   0xfdff
#define ECHO_RAM_LENGTH GROUP_LENGTH(ECHO_RAM)

#define SPRITE_RAM_START 0xfe00
#define SPRITE_RAM_END   0xfe9f
#define SPRITE_RAM_LENGTH GROUP_LENGTH(SPRITE_RAM)

#define UNUSED_AREA_START 0xfea0
#define UNUSED_AREA_END   0xfeff
#define UNUSED_AREA_LENGTH GROUP_LENGTH(UNUSED_AREA)

#define IO_CHANNEL_START 0xff00
#define IO_CHANNEL_END   0xff7f
#define IO_CHANNEL_LENGTH GROUP_LENGTH(IO_CHANNEL)

#define HIGH_RAM_START 0xff80
#define HIGH_RAM_END   0xfffe
#define HIGH_RAM_LENGTH GROUP_LENGTH(HIGH_RAM)

#define INERRUPT_ENABLE_REGISTER 0xffff

class Motherboard {
public:
    typedef std::shared_ptr<Motherboard> Ptr;

    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t value) = 0;

    uint16_t readLI(uint16_t addr);
    void writeLI(uint16_t addr, uint16_t value);
};

#endif // __Memory_h__
