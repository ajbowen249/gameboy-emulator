#include "Motherboard.h"

#include "Util.h"

uint16_t Motherboard::readLI(uint16_t addr) {
	uint8_t lowByte = read(addr);
	uint8_t highByte = read(addr + 1);

	return TO_16(highByte, lowByte);
}

void Motherboard::writeLI(uint16_t addr, uint16_t value) {
	uint8_t highByte, lowByte;
	FROM_16(highByte, lowByte, value);

	write(addr, lowByte);
	write(addr + 1, highByte);
}
