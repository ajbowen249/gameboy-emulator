#ifndef __Cartridge_h__
#define __Cartridge_h__

#include <cstdint>
#include <memory>
#include <vector>

#define TITLE_START 0x0134
#define MAX_TITLE_LENGTH 16

#define GBC_FLAG_ADDR 0x0143
#define SGB_FLAG_ADDR 0x0146
#define CART_TYPE_ADDR 0x0147
#define ROM_SIZE_ADDR 0x0148
#define RAM_SIZE_ADDR 0x0149

enum CartridgeType : uint8_t {
    ROM_ONLY = 0x00,
    ROM_MBC1 = 0x01,
    ROM_MBC1_RAM = 0x02,
    ROM_MBC1_RAM_BATT = 0x03,
    ROM_MBC2 = 0x05,
    ROM_MBC2_BATTERY = 0x06,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    ROM_MMM01 = 0x0b,
    ROM_MMM01_SRAM = 0x0c,
    ROM_MMM01_SRAM_BATT = 0x0d,
    ROM_MBC3_RAM = 0x12,
    ROM_MBC3_RAM_BATT = 0x13,
    ROM_MBC5 = 0x19,
    ROM_MBC5_RAM = 0x1a,
    ROM_MBC5_RAM_BATT = 0x1b,
    ROM_MBC5_RUMBLE = 0x1c,
    ROM_MBC5_RUMBLE_SRAM = 0x1d,
    ROM_MBC5_RUMBLE_SRAM_BATT = 0x1e,
    CAMERA = 0x1f,
    TAMA5 = 0xfd,
    HUC_3 = 0xfe,
};

class Cartridge {
public:
    typedef std::shared_ptr<Cartridge> Ptr;
    Cartridge(const std::vector<uint8_t>&& rawBytes);

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    std::vector<uint8_t> _rawBytes;

    CartridgeType _cartridgeType;
    uint8_t _numRomBanks;
    uint8_t _numRamBanks;
    uint16_t _ramSize;
};

#endif // __Cartridge_h__
