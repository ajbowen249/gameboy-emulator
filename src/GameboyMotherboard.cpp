#include "GameboyMotherboard.h"

uint8_t GameboyMotherboard::read(uint16_t addr) {
    if (addr <= ROM_BANK_N_END) {
        return _cartridge->read(addr);
    } else if (addr >= TILE_RAM_START && addr <= TILE_RAM_END) {
        return _tileRam[addr - TILE_RAM_START];
    } else if (addr >= BACKGROUND_DATA_1_START && addr <= BACKGROUND_DATA_1_END) {
        return _bgData1[addr - BACKGROUND_DATA_1_START];
    } else if (addr >= BACKGROUND_DATA_2_START && addr <= BACKGROUND_DATA_2_END) {
        return _bgData2[addr - BACKGROUND_DATA_2_START];
    } else if (addr >= CART_RAM_START && addr <= CART_RAM_END) {
        return _cartridge->read(addr);
    } else if (addr >= WORK_RAM_START && addr <= WORK_RAM_END) {
        return _workRam[addr - WORK_RAM_START];
    } else if (addr >= ECHO_RAM_START && addr <= ECHO_RAM_END) {
        return _workRam[addr - ECHO_RAM_START];
    } else if (addr >= SPRITE_RAM_START && addr <= SPRITE_RAM_END) {
        return _spriteRam[addr - SPRITE_RAM_START];
    } else if (addr >= IO_CHANNEL_START && addr <= IO_CHANNEL_END) {
        return 0; // TODO: IO Channel
    } else if (addr >= HIGH_RAM_START && addr <= HIGH_RAM_END) {
        return _highRam[addr - HIGH_RAM_START];
    } else if (addr == INERRUPT_ENABLE_REGISTER) {
        return _interruptEnableRegister;
    }

    return 0;
}

void GameboyMotherboard::write(uint16_t addr, uint8_t value) {
    if (addr <= ROM_BANK_N_END) {
        _cartridge->write(addr, value);
    } else if (addr >= TILE_RAM_START && addr <= TILE_RAM_END) {
        _tileRam[addr - TILE_RAM_START] = value;
    } else if (addr >= BACKGROUND_DATA_1_START && addr <= BACKGROUND_DATA_1_END) {
        _bgData1[addr - BACKGROUND_DATA_1_START] = value;
    } else if (addr >= BACKGROUND_DATA_2_START && addr <= BACKGROUND_DATA_2_END) {
        _bgData2[addr - BACKGROUND_DATA_2_START] = value;
    } else if (addr >= CART_RAM_START && addr <= CART_RAM_END) {
        _cartridge->write(addr, value);
    } else if (addr >= WORK_RAM_START && addr <= WORK_RAM_END) {
        _workRam[addr - WORK_RAM_START] = value;
    } else if (addr >= ECHO_RAM_START && addr <= ECHO_RAM_END) {
        _workRam[addr - ECHO_RAM_START] = value;
    } else if (addr >= SPRITE_RAM_START && addr <= SPRITE_RAM_END) {
        _spriteRam[addr - SPRITE_RAM_START] = value;
    } else if (addr >= IO_CHANNEL_START && addr <= IO_CHANNEL_END) {
        // TODO: IO Channel
    } else if (addr >= HIGH_RAM_START && addr <= HIGH_RAM_END) {
        _highRam[addr - HIGH_RAM_START] = value;
    } else if (addr == INERRUPT_ENABLE_REGISTER) {
        _interruptEnableRegister = value;
    }
}

void GameboyMotherboard::setCartridge(Cartridge::Ptr cartridge) {
    _cartridge = cartridge;
}
