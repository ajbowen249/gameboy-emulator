#ifndef __Opcodes_h__
#define __Opcodes_h__

#include <cstdint>

enum Opcode : uint8_t {
    // Load Immediate
    LD_B_n = 0x06,
    LD_C_n = 0x0E,
    LD_D_n = 0x16,
    LD_E_n = 0x1E,
    LD_H_n = 0x26,
    LD_L_n = 0x2e,
};

#endif // __Opcodes_h__
