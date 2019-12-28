#ifndef __Util_h__
#define __Util_h__

#include <cstdint>

#define TO_16(highByte, lowByte) \
    ((uint16_t)highByte << 8) | (0x00ff & lowByte) \

#define FROM_16(highByte, lowByte, value) \
    highByte = (uint8_t)((value & 0xff00) >> 8); \
    lowByte = (uint8_t)(value & 0x00ff); \

#endif //__Util_h__
