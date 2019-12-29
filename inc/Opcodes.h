#ifndef __Opcodes_h__
#define __Opcodes_h__

#include <cstdint>

enum Opcode : uint8_t {
    // Load Immediate
    LD_A_n = 0x3e,
    LD_B_n = 0x06,
    LD_C_n = 0x0e,
    LD_D_n = 0x16,
    LD_E_n = 0x1e,
    LD_H_n = 0x26,
    LD_L_n = 0x2e,

    // Load Register->Register
    LD_A_A = 0x7f,
    LD_A_B = 0x78,
    LD_A_C = 0x79,
    LD_A_D = 0x7a,
    LD_A_E = 0x7b,
    LD_A_H = 0x7c,
    LD_A_L = 0x7d,

    LD_B_A = 0x47,
    LD_B_B = 0x40,
    LD_B_C = 0x41,
    LD_B_D = 0x42,
    LD_B_E = 0x43,
    LD_B_H = 0x44,
    LD_B_L = 0x45,

    LD_C_A = 0x4f,
    LD_C_B = 0x48,
    LD_C_C = 0x49,
    LD_C_D = 0x4a,
    LD_C_E = 0x4b,
    LD_C_H = 0x4c,
    LD_C_L = 0x4d,

    LD_D_A = 0x57,
    LD_D_B = 0x50,
    LD_D_C = 0x51,
    LD_D_D = 0x52,
    LD_D_E = 0x53,
    LD_D_H = 0x54,
    LD_D_L = 0x55,

    LD_E_A = 0x5f,
    LD_E_B = 0x58,
    LD_E_C = 0x59,
    LD_E_D = 0x5A,
    LD_E_E = 0x5B,
    LD_E_H = 0x5C,
    LD_E_L = 0x5d,

    LD_H_A = 0x67,
    LD_H_B = 0x60,
    LD_H_C = 0x61,
    LD_H_D = 0x62,
    LD_H_E = 0x63,
    LD_H_H = 0x64,
    LD_H_L = 0x65,

    LD_L_A = 0x6f,
    LD_L_B = 0x68,
    LD_L_C = 0x69,
    LD_L_D = 0x6a,
    LD_L_E = 0x6b,
    LD_L_H = 0x6c,
    LD_L_L = 0x6d,

    LD_HL_SP = 0xf9,

    // Load Address->Register
    LD_A_aHL =   0x7e,
    LD_B_aHL =   0x46,
    LD_C_aHL =   0x4e,
    LD_D_aHL =   0x56,
    LD_E_aHL =   0x5E,
    LD_H_aHL =   0x66,
    LD_L_aHL =   0x6E,
    LD_A_aBC =   0x0a,
    LD_A_aDE =   0x1a,
    LD_A_aNN =   0xfa,
    LD_A_afC =   0xf2,
    LDD_A_aHL =  0x3a,
    LDI_A_aHL =  0x2a,
    LDH_A_afN =  0xf0,

    // "Load" Register->Address
    LD_aHL_A =  0x77,
    LD_aHL_B =  0x70,
    LD_aHL_C =  0x71,
    LD_aHL_D =  0x72,
    LD_aHL_E =  0x73,
    LD_aHL_H =  0x74,
    LD_aHL_L =  0x75,
    LD_aBC_A =  0x02,
    LD_aDE_A =  0x12,
    LD_aNN_A =  0xea,
    LD_afC_A =  0xe2,
    LDD_aHL_A = 0x32,
    LDI_aHL_A = 0x22,
    LDH_afN_A = 0xe0,

    // "Load" Constant->Address
    LD_aHL_n = 0x36,

    // 16-bit Immediate Assignment
    LD_BC_NN = 0x01,
    LD_DE_NN = 0x11,
    LD_HL_NN = 0x21,
    LD_SP_NN = 0x31,

    // Load HL with value at SP+n
    LD_HL_aSPN = 0xf8,

    // Store to address NN the stack pointer
    LD_aNN_SP = 0x08,

    // Push
    PUSH_AF = 0xf5,
    PUSH_BC = 0xc5,
    PUSH_DE = 0xd5,
    PUSH_HL = 0xe5,

    // Pop
    POP_AF = 0xf1,
    POP_BC = 0xc1,
    POP_DE = 0xd1,
    POP_HL = 0xe1,

    // 8-bit Add
    ADD_A_A =   0x87,
    ADD_A_B =   0x80,
    ADD_A_C =   0x81,
    ADD_A_D =   0x82,
    ADD_A_E =   0x83,
    ADD_A_H =   0x84,
    ADD_A_L =   0x85,
    ADD_A_aHL = 0x86,
    ADD_A_N  =  0xc6,

    // 8-bit Add with Carry
    ADC_A_A =   0x8f,
    ADC_A_B =   0x88,
    ADC_A_C =   0x89,
    ADC_A_D =   0x8a,
    ADC_A_E =   0x8b,
    ADC_A_H =   0x8c,
    ADC_A_L =   0x8d,
    ADC_A_aHL = 0x8e,
    ADC_A_N =   0xce,

    // 8-bit Subtract
    SUB_A =   0x97,
    SUB_B =   0x90,
    SUB_C =   0x91,
    SUB_D =   0x92,
    SUB_E =   0x93,
    SUB_H =   0x94,
    SUB_L =   0x95,
    SUB_aHL = 0x96,
    SUB_N =   0xd6,

    // 8-bit Subtract with Carry
    SBC_A_A =   0x9f,
    SBC_A_B =   0x98,
    SBC_A_C =   0x99,
    SBC_A_D =   0x9a,
    SBC_A_E =   0x9b,
    SBC_A_H =   0x9c,
    SBC_A_L =   0x9d,
    SBC_A_aHL = 0x9e,
    SBC_A_N =   0xde,
};

#endif // __Opcodes_h__
