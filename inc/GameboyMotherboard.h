#ifndef __GameboyMotherboard_H__
#define __GameboyMotherboard_H__

#include "Cartridge.h"
#include "Motherboard.h"

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

    // Control registers (not everything is used)
    uint8_t _ioChannel[IO_CHANNEL_LENGTH];

    // Upper area of work ram
    uint8_t _highRam[HIGH_RAM_LENGTH];

    uint8_t _interruptEnableRegister;

    Cartridge::Ptr _cartridge;
};

#endif // __GameboyMotherboard_H__
