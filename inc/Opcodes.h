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

    // Load Register->Register
    LD_A_A = 0x7F,
    LD_A_B = 0x78,
    LD_A_C = 0x79,
    LD_A_D = 0x7A,
    LD_A_E = 0x7B,
    LD_A_H = 0x7C,
    LD_A_L = 0x7D,
    LD_A_(HL) 7E 8
    LD_B_B 40 4
    LD_B_C 41 4
    LD_B_D 42 4
    LD_B_E 43 4
    LD_B_H 44 4
    LD_B_L 45 4
    LD_B_(HL) 46 8
    LD_C_B 48 4
    LD_C_C 49 4
    LD_C_D 4A 4
    LD_C_E 4B 4
    LD_C_H 4C 4
    LD_C_L 4D 4
    LD_C_(HL) 4E 8
    LD_D_B 50 4
    LD_D_C 51 4
};

#endif // __Opcodes_h__
