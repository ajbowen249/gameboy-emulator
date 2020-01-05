#ifndef __GameboyMotherboard_H__
#define __GameboyMotherboard_H__

#include "Cartridge.h"
#include "Motherboard.h"

#define length(name) (name##_END - name##_START) + 0x0001

#define ROM_BANK_0_START 0x0000
#define ROM_BANK_0_END   0x3fff
#define ROM_BANK_0_LENGTH length(ROM_BANK_0)

#define ROM_BANK_N_START 0x4000
#define ROM_BANK_N_END   0x7fff
#define ROM_BANK_N_LENGTH length(ROM_BANK_N)

#define TILE_RAM_START 0x8000
#define TILE_RAM_END   0x97ff
#define TILE_RAM_LENGTH length(TILE_RAM)

#define BACKGROUND_DATA_1_START 0x9800
#define BACKGROUND_DATA_1_END   0x9bff
#define BACKGROUND_DATA_1_LENGTH length(BACKGROUND_DATA_1)

#define BACKGROUND_DATA_2_START 0x9c00
#define BACKGROUND_DATA_2_END   0x9fff
#define BACKGROUND_DATA_2_LENGTH length(BACKGROUND_DATA_2)

#define CART_RAM_START 0xa000
#define CART_RAM_END   0xbfff
#define CART_RAM_LENGTH length(CART_RAM)

#define WORK_RAM_START 0xc000
#define WORK_RAM_END   0xdfff
#define WORK_RAM_LENGTH length(WORK_RAM)

#define ECHO_RAM_START 0xe000
#define ECHO_RAM_END   0xfdff
#define ECHO_RAM_LENGTH length(ECHO_RAM)

#define SPRITE_RAM_START 0xfe00
#define SPRITE_RAM_END   0xfe9f
#define SPRITE_RAM_LENGTH length(SPRITE_RAM)

#define UNUSED_AREA_START 0xfea0
#define UNUSED_AREA_END   0xfeff
#define UNUSED_AREA_LENGTH length(UNUSED_AREA)

#define IO_CHANNEL_START 0xff00
#define IO_CHANNEL_END   0xff7f
#define IO_CHANNEL_LENGTH length(IO_CHANNEL)

#define HIGH_RAM_START 0xff80
#define HIGH_RAM_END   0xfffe
#define HIGH_RAM_LENGTH length(HIGH_RAM)

#define INERRUPT_ENABLE_REGISTER 0xffff

class GameboyMotherboard : public Motherboard {
public:
    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t value) override;

    void setCartridge(Cartridge::Ptr cartridge);

private:
    // Video Memory
    uint8_t _tileRam[TILE_RAM_LENGTH];
    uint8_t _bgData1[BACKGROUND_DATA_1_LENGTH];
    uint8_t _bgData2[BACKGROUND_DATA_2_LENGTH];

    // General-purpose internal RAM
    uint8_t _workRam[WORK_RAM_LENGTH];

    // Sprite attributes table
    uint8_t _spriteRam[SPRITE_RAM_LENGTH];

    // Upper area of work ram
    uint8_t _highRam[HIGH_RAM_LENGTH];

    uint8_t _interruptEnableRegister;

    Cartridge::Ptr _cartridge;
};

#endif // __GameboyMotherboard_H__
