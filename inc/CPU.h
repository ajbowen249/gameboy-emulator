#ifndef __CPU_h_
#define __CPU_h_

#include <cstdint>

#include "Memory.h"

#define INIT_VECTOR 0x0100
#define INIT_STACK_POINTER 0xfffe
#define CLOCK_CYCLES_PER_MACHINE_CYCLE 4

#define RET_COMBO_REGISTER(reg1, reg2) \
    return ((uint16_t)reg1 << 8) | (0x00ff & reg2) \

#define SET_COMBO_REGISTER(reg1, reg2) \
    reg1 = (uint8_t)(value >> 8); reg2 = (uint8_t)(value & 0x00ff); \

#define FLAG_TO_BOOL(mask) \
    return (_flags & mask) != 0 \

#define SET_FLAG(value, mask) \
    _flags = value ? _flags | mask : _flags & (~mask)

class CPU {
public:
    CPU(Memory::Ptr memory);

    /** Zero out flags, reset the PC and SP.
     */
    void reset();

    /** Simulates an actual tick of the crystal.
     *
     * In practice, this means that every fourth invocation of this function
     * leads to a single machine cycle execution.
     */
    void clock();

    // IMPROVE: I would normally consider these to be private. However, for
    //          sake of tests and creating debug/observation tools, they are
    //          public for now.
    uint8_t _regA;
    uint8_t _regB;
    uint8_t _regC;
    uint8_t _regD;
    uint8_t _regE;
    uint8_t _regH;
    uint8_t _regL;

    uint16_t _stackPointer;
    uint16_t _programCounter;

    uint8_t _flags;

    inline uint16_t regAF() const { RET_COMBO_REGISTER(_regA, _flags); }
    inline uint16_t regBC() const { RET_COMBO_REGISTER(_regB, _regC); }
    inline uint16_t regDE() const { RET_COMBO_REGISTER(_regD, _regE); }
    inline uint16_t regHL() const { RET_COMBO_REGISTER(_regH, _regL); }

    inline void regAF(uint16_t value) { SET_COMBO_REGISTER(_regA, _flags); }
    inline void regBC(uint16_t value) { SET_COMBO_REGISTER(_regB, _regC); }
    inline void regDE(uint16_t value) { SET_COMBO_REGISTER(_regD, _regE); }
    inline void regHL(uint16_t value) { SET_COMBO_REGISTER(_regH, _regL); }

    inline bool zFlag() const { FLAG_TO_BOOL(0x80); }
    inline bool nFlag() const { FLAG_TO_BOOL(0x40); }
    inline bool hFlag() const { FLAG_TO_BOOL(0x20); }
    inline bool cFlag() const { FLAG_TO_BOOL(0x10); }

    inline void zFlag(bool value) { SET_FLAG(value, 0x80); }
    inline void nFlag(bool value) { SET_FLAG(value, 0x40); }
    inline void hFlag(bool value) { SET_FLAG(value, 0x20); }
    inline void cFlag(bool value) { SET_FLAG(value, 0x10); }

private:
    Memory::Ptr _memory;
    int8_t _awaitingClockCycles;
    int8_t _awaitingMachineCycles;

    void machineCycle();
    int8_t decodeAndExecute();

    // Instruction functions return the number of machine cycles they should
    // take to execute.

    int8_t I_LoadImmediate(uint8_t opcode);
    int8_t I_TransferRegister(uint8_t opcode);
    int8_t I_LoadAddressIntoRegister(uint8_t opcode);
    int8_t I_StoreToAddress(uint8_t opcode);
    int8_t I_LoadImmediate16(uint8_t opcode);
    int8_t I_LoadHLWithSPN();
    int8_t I_StoreStackPointer();
    int8_t I_PushRegister(uint8_t opcode);
    int8_t I_PopRegister(uint8_t opcode);
    int8_t I_8BitAdd(uint8_t opcode);
    int8_t I_8BitSubtract(uint8_t opcode);
    int8_t I_And(uint8_t opcode);
    int8_t I_Or(uint8_t opcode);
    int8_t I_Xor(uint8_t opcode);
    int8_t I_Compare(uint8_t opcode);
    int8_t I_Increment(uint8_t opcode);
    int8_t I_Decrement(uint8_t opcode);
    int8_t I_16BitAdd(uint8_t opcode);
    int8_t I_AddToSP();
    int8_t I_16BitIncrement(uint8_t opcode);
    int8_t I_16BitDecrement(uint8_t opcode);
    int8_t I_DecimalAdjust();
    int8_t I_ComplementA();
    int8_t I_ComplementCarry();
    int8_t I_SetCarry();
    int8_t I_UnconditionalJump();
    int8_t I_ConditionalJump(uint8_t opcode);
    int8_t I_JumpToHL();
    int8_t I_UnconditionalRelativeJump();
    int8_t I_ConditionalRelativeJump(uint8_t opcode);
};

#endif // __CPU_h_
