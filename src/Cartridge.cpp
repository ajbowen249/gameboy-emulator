#include "Cartridge.h"

Cartridge::Cartridge(const std::vector<uint8_t>&& rawBytes) :
    _rawBytes(std::move(rawBytes)),
    _cartridgeType((CartridgeType)_rawBytes[CART_TYPE_ADDR]) {
    // TODO: Determine other cart attributes.
}

uint8_t Cartridge::read(uint16_t address) {
    // WARNING: Only supporting cart type 0 for now
    return _rawBytes[(size_t)address];
}

void Cartridge::write(uint16_t address, uint8_t value) {
    // WARNING: Only supporting cart type 0 for now
}
