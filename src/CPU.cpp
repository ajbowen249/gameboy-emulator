#include "CPU.h"

#include "Log.h"
#include "Opcodes.h"
#include "Util.h"

#include <sstream>

CPU::CPU(Memory::Ptr memory) : _memory(memory) {
    reset();
}

void CPU::reset() {
    _programCounter = INIT_VECTOR;
    _stackPointer = INIT_STACK_POINTER;
    _flags = 0;

    _awaitingClockCycles = CLOCK_CYCLES_PER_MACHINE_CYCLE;
    _awaitingMachineCycles = 0;
}

void CPU::clock() {
    // IMPROVE: For now, clock cycles just wait in an attempt to keep timing
    //          accurate. A better implementation would allow the clock cycle
    //          to drive machine state.
    if (--_awaitingClockCycles <= 0) {
        _awaitingClockCycles = CLOCK_CYCLES_PER_MACHINE_CYCLE;
        machineCycle();
    }
}

void CPU::machineCycle() {
    // IMPROVE: For now, machine cycles just wait in an attempt to keep timing
    //          accurate. A better implementation would split up machine states
    //          for accurate internals.
    if (_awaitingMachineCycles > 0) {
        _awaitingMachineCycles--;
        return;
    }

    _awaitingMachineCycles = decodeAndExecute() - 1;
}

int8_t CPU::decodeAndExecute() {
    uint8_t opcode = _memory->read(_programCounter++);

    switch(opcode) {
        case Opcode::LD_A_n:
        case Opcode::LD_B_n:
        case Opcode::LD_C_n:
        case Opcode::LD_D_n:
        case Opcode::LD_E_n:
        case Opcode::LD_H_n:
        case Opcode::LD_L_n:
            return I_LoadImmediate(opcode);
        case Opcode::LD_A_A:
        case Opcode::LD_A_B:
        case Opcode::LD_A_C:
        case Opcode::LD_A_D:
        case Opcode::LD_A_E:
        case Opcode::LD_A_H:
        case Opcode::LD_A_L:
        case Opcode::LD_B_A:
        case Opcode::LD_B_B:
        case Opcode::LD_B_C:
        case Opcode::LD_B_D:
        case Opcode::LD_B_E:
        case Opcode::LD_B_H:
        case Opcode::LD_B_L:
        case Opcode::LD_C_A:
        case Opcode::LD_C_B:
        case Opcode::LD_C_C:
        case Opcode::LD_C_D:
        case Opcode::LD_C_E:
        case Opcode::LD_C_H:
        case Opcode::LD_C_L:
        case Opcode::LD_D_A:
        case Opcode::LD_D_B:
        case Opcode::LD_D_C:
        case Opcode::LD_D_D:
        case Opcode::LD_D_E:
        case Opcode::LD_D_H:
        case Opcode::LD_D_L:
        case Opcode::LD_E_A:
        case Opcode::LD_E_B:
        case Opcode::LD_E_C:
        case Opcode::LD_E_D:
        case Opcode::LD_E_E:
        case Opcode::LD_E_H:
        case Opcode::LD_E_L:
        case Opcode::LD_H_A:
        case Opcode::LD_H_B:
        case Opcode::LD_H_C:
        case Opcode::LD_H_D:
        case Opcode::LD_H_E:
        case Opcode::LD_H_H:
        case Opcode::LD_H_L:
        case Opcode::LD_L_A:
        case Opcode::LD_L_B:
        case Opcode::LD_L_C:
        case Opcode::LD_L_D:
        case Opcode::LD_L_E:
        case Opcode::LD_L_H:
        case Opcode::LD_L_L:
        case Opcode::LD_HL_SP:
            return I_TransferRegister(opcode);
        case Opcode::LD_A_aHL:
        case Opcode::LD_B_aHL:
        case Opcode::LD_C_aHL:
        case Opcode::LD_D_aHL:
        case Opcode::LD_E_aHL:
        case Opcode::LD_H_aHL:
        case Opcode::LD_L_aHL:
        case Opcode::LD_A_aBC:
        case Opcode::LD_A_aDE:
        case Opcode::LD_A_aNN:
        case Opcode::LD_A_afC:
        case Opcode::LDD_A_aHL:
        case Opcode::LDI_A_aHL:
        case Opcode::LDH_A_afN:
            return I_LoadAddressIntoRegister(opcode);
        case Opcode::LD_aHL_A:
        case Opcode::LD_aHL_B:
        case Opcode::LD_aHL_C:
        case Opcode::LD_aHL_D:
        case Opcode::LD_aHL_E:
        case Opcode::LD_aHL_H:
        case Opcode::LD_aHL_L:
        case Opcode::LD_aHL_n:
        case Opcode::LD_aBC_A:
        case Opcode::LD_aDE_A:
        case Opcode::LD_aNN_A:
        case Opcode::LD_afC_A:
        case Opcode::LDD_aHL_A:
        case Opcode::LDI_aHL_A:
        case Opcode::LDH_afN_A:
            return I_StoreToAddress(opcode);
        case Opcode::LD_BC_NN:
        case Opcode::LD_DE_NN:
        case Opcode::LD_HL_NN:
        case Opcode::LD_SP_NN:
            return I_LoadImmediate16(opcode);
        case Opcode::LD_HL_aSPN:
            return I_LoadHLWithSPN();
        case Opcode::LD_aNN_SP:
            return I_StoreStackPointer();
        case Opcode::PUSH_AF:
        case Opcode::PUSH_BC:
        case Opcode::PUSH_DE:
        case Opcode::PUSH_HL:
            return I_PushRegister(opcode);
        case Opcode::POP_AF:
        case Opcode::POP_BC:
        case Opcode::POP_DE:
        case Opcode::POP_HL:
            return I_PopRegister(opcode);
        case Opcode::ADD_A_A:
        case Opcode::ADD_A_B:
        case Opcode::ADD_A_C:
        case Opcode::ADD_A_D:
        case Opcode::ADD_A_E:
        case Opcode::ADD_A_H:
        case Opcode::ADD_A_L:
        case Opcode::ADD_A_aHL:
        case Opcode::ADD_A_N:
        case Opcode::ADC_A_A:
        case Opcode::ADC_A_B:
        case Opcode::ADC_A_C:
        case Opcode::ADC_A_D:
        case Opcode::ADC_A_E:
        case Opcode::ADC_A_H:
        case Opcode::ADC_A_L:
        case Opcode::ADC_A_aHL:
        case Opcode::ADC_A_N:
            return I_8BitAdd(opcode);
        case Opcode::SUB_A:
        case Opcode::SUB_B:
        case Opcode::SUB_C:
        case Opcode::SUB_D:
        case Opcode::SUB_E:
        case Opcode::SUB_H:
        case Opcode::SUB_L:
        case Opcode::SUB_aHL:
        case Opcode::SUB_N:
        case Opcode::SBC_A_A:
        case Opcode::SBC_A_B:
        case Opcode::SBC_A_C:
        case Opcode::SBC_A_D:
        case Opcode::SBC_A_E:
        case Opcode::SBC_A_H:
        case Opcode::SBC_A_L:
        case Opcode::SBC_A_aHL:
        case Opcode::SBC_A_N:
            return I_8BitSubtract(opcode);
        case Opcode::AND_A:
        case Opcode::AND_B:
        case Opcode::AND_C:
        case Opcode::AND_D:
        case Opcode::AND_E:
        case Opcode::AND_H:
        case Opcode::AND_L:
        case Opcode::AND_aHL:
        case Opcode::AND_N:
            return I_And(opcode);
        case Opcode::OR_A:
        case Opcode::OR_B:
        case Opcode::OR_C:
        case Opcode::OR_D:
        case Opcode::OR_E:
        case Opcode::OR_H:
        case Opcode::OR_L:
        case Opcode::OR_aHL:
        case Opcode::OR_N:
            return I_Or(opcode);
        case Opcode::XOR_A:
        case Opcode::XOR_B:
        case Opcode::XOR_C:
        case Opcode::XOR_D:
        case Opcode::XOR_E:
        case Opcode::XOR_H:
        case Opcode::XOR_L:
        case Opcode::XOR_aHL:
        case Opcode::XOR_N:
            return I_Xor(opcode);
    }
}

int8_t CPU::I_LoadImmediate(uint8_t opcode) {
    uint8_t value = _memory->read(_programCounter++);

    switch(opcode) {
        case Opcode::LD_A_n: _regA = value; break;
        case Opcode::LD_B_n: _regB = value; break;
        case Opcode::LD_C_n: _regC = value; break;
        case Opcode::LD_D_n: _regD = value; break;
        case Opcode::LD_E_n: _regE = value; break;
        case Opcode::LD_H_n: _regH = value; break;
        case Opcode::LD_L_n: _regL = value; break;
    }

    return  2;
}

#define REG_TRANSFER_GROUP(reg) \
    case Opcode::LD_##reg##_A: _reg##reg = _regA; break; \
    case Opcode::LD_##reg##_B: _reg##reg = _regB; break; \
    case Opcode::LD_##reg##_C: _reg##reg = _regC; break; \
    case Opcode::LD_##reg##_D: _reg##reg = _regD; break; \
    case Opcode::LD_##reg##_E: _reg##reg = _regE; break; \
    case Opcode::LD_##reg##_H: _reg##reg = _regH; break; \
    case Opcode::LD_##reg##_L: _reg##reg = _regL; break; \

int8_t CPU::I_TransferRegister(uint8_t opcode) {
    switch(opcode) {
        REG_TRANSFER_GROUP(A);
        REG_TRANSFER_GROUP(B);
        REG_TRANSFER_GROUP(C);
        REG_TRANSFER_GROUP(D);
        REG_TRANSFER_GROUP(E);
        REG_TRANSFER_GROUP(H);
        REG_TRANSFER_GROUP(L);
        case Opcode::LD_HL_SP:
            _stackPointer = regHL();
            return 2;
    }

    return 1;
}

int8_t CPU::I_LoadAddressIntoRegister(uint8_t opcode) {
    if (opcode == Opcode::LD_A_afC) {
        _regA = _memory->read(0xff00 + (uint16_t)_regC);
        return 2;
    } else if (opcode == Opcode::LDH_A_afN) {
        const auto indirectAddress = _memory->read(_programCounter++);
        _regA = _memory->read(0xff00 + (uint16_t)indirectAddress);
        return 3;
    }

    switch(opcode) {
        case Opcode::LD_A_aBC:
            _regA = _memory->read(regBC());
            return 2;
        case Opcode::LD_A_aDE:
            _regA = _memory->read(regDE());
            return 2;
        case Opcode::LD_A_aNN:
            const auto addr = _memory->readLI(_programCounter);
            _programCounter += 2;
            _regA = _memory->read(addr);
            return 4;
    }

    const auto value = _memory->read(regHL());

    if (opcode == Opcode::LDD_A_aHL) {
        regHL(regHL() - 1);
    } else if (opcode == Opcode::LDI_A_aHL) {
        regHL(regHL() + 1);
    }

    switch(opcode) {
        case Opcode::LDI_A_aHL:
        case Opcode::LDD_A_aHL:
        case Opcode::LD_A_aHL: _regA = value; break;
        case Opcode::LD_B_aHL: _regB = value; break;
        case Opcode::LD_C_aHL: _regC = value; break;
        case Opcode::LD_D_aHL: _regD = value; break;
        case Opcode::LD_E_aHL: _regE = value; break;
        case Opcode::LD_H_aHL: _regH = value; break;
        case Opcode::LD_L_aHL: _regL = value; break;
    }

    return 2;
}

int8_t CPU::I_StoreToAddress(uint8_t opcode) {
    if (opcode == Opcode::LD_afC_A) {
        _memory->write(0xff00 + (uint16_t)_regC, _regA);
        return 2;
    } else if (opcode == Opcode::LDH_afN_A) {
        const auto indirectAddress = _memory->read(_programCounter++);
        _memory->write(0xff00 + (uint16_t)indirectAddress, _regA);
        return 3;
    }

    switch(opcode) {
        case Opcode::LD_aBC_A:
            _memory->write(regBC(), _regA);
            return 2;
        case Opcode::LD_aDE_A:
            _memory->write(regDE(), _regA);
            return 2;
        case Opcode::LD_aNN_A:
            const auto addr = _memory->readLI(_programCounter);
            _programCounter += 2;
            _memory->write(addr, _regA);
            return 4;
    }

    uint8_t value = 0;
    switch(opcode) {
        case Opcode::LDD_aHL_A:
        case Opcode::LDI_aHL_A:
        case Opcode::LD_aHL_A: value = _regA; break;
        case Opcode::LD_aHL_B: value = _regB; break;
        case Opcode::LD_aHL_C: value = _regC; break;
        case Opcode::LD_aHL_D: value = _regD; break;
        case Opcode::LD_aHL_E: value = _regE; break;
        case Opcode::LD_aHL_H: value = _regH; break;
        case Opcode::LD_aHL_L: value = _regL; break;
        case Opcode::LD_aHL_n:
            value = _memory->read(_programCounter++);
            break;
    }

    _memory->write(regHL(), value);

    if (opcode == Opcode::LDD_aHL_A) {
        regHL(regHL() - 1);
    } else if (opcode == Opcode::LDI_aHL_A) {
        regHL(regHL() + 1);
    }

    return opcode == Opcode::LD_aHL_n ? 3 : 2;
}

int8_t CPU::I_LoadImmediate16(uint8_t opcode) {
    const auto value = _memory->readLI(_programCounter);
    _programCounter += 2;

    switch(opcode) {
        case Opcode::LD_BC_NN: regBC(value); break;
        case Opcode::LD_DE_NN: regDE(value); break;
        case Opcode::LD_HL_NN: regHL(value); break;
        case Opcode::LD_SP_NN: _stackPointer = value; break;
    }

    return 3;
}

int8_t CPU::I_LoadHLWithSPN() {
    auto rawOffset = _memory->read(_programCounter++);
    auto offset = *reinterpret_cast<int8_t*>(&rawOffset);
    const auto effectiveAddress = _stackPointer + (int16_t)offset;
    // Little-endian load
    _regL = _memory->read(effectiveAddress);
    _regH = _memory->read(effectiveAddress + 1);

    zFlag(false);
    nFlag(false);

    // Flag logic lifted from here:
    // https://stackoverflow.com/questions/5159603/gbz80-how-does-ld-hl-spe-affect-h-and-c-flags
    // Official documentation is pretty light on the topic.
    if (offset > 0) {
        cFlag(((_stackPointer & 0xff) + offset) > 0xff);
        hFlag(((_stackPointer & 0xf) + (offset & 0xf)) > 0xf);
    } else {
        cFlag((_stackPointer & 0xff) <= (_stackPointer & 0xff));
        hFlag((_stackPointer & 0xf) <= (_stackPointer & 0xf));
    }

    return 3;
}

int8_t CPU::I_StoreStackPointer() {
    const auto address = _memory->readLI(_programCounter);
    _programCounter += 2;

    _memory->writeLI(address, _stackPointer);

    return 5;
}

int8_t CPU::I_PushRegister(uint8_t opcode) {
    uint16_t value = 0;
    switch (opcode) {
        case Opcode::PUSH_AF: value = regAF(); break;
        case Opcode::PUSH_BC: value = regBC(); break;
        case Opcode::PUSH_DE: value = regDE(); break;
        case Opcode::PUSH_HL: value = regHL(); break;
    }

    _stackPointer -= 2;

    _memory->writeLI(_stackPointer, value);

    return 4;
}

int8_t CPU::I_PopRegister(uint8_t opcode) {
    uint16_t value = _memory->readLI(_stackPointer);
    _stackPointer += 2;

    switch (opcode) {
        case Opcode::POP_AF: regAF(value); break;
        case Opcode::POP_BC: regBC(value); break;
        case Opcode::POP_DE: regDE(value); break;
        case Opcode::POP_HL: regHL(value); break;
    }

    return 3;
}

int8_t CPU::I_8BitAdd(uint8_t opcode) {
    uint8_t operand = 0;
    int8_t cycles = 1;

    switch(opcode) {
        case Opcode::ADD_A_A: case Opcode::ADC_A_A: operand = _regA; break;
        case Opcode::ADD_A_B: case Opcode::ADC_A_B: operand = _regB; break;
        case Opcode::ADD_A_C: case Opcode::ADC_A_C: operand = _regC; break;
        case Opcode::ADD_A_D: case Opcode::ADC_A_D: operand = _regD; break;
        case Opcode::ADD_A_E: case Opcode::ADC_A_E: operand = _regE; break;
        case Opcode::ADD_A_H: case Opcode::ADC_A_H: operand = _regH; break;
        case Opcode::ADD_A_L: case Opcode::ADC_A_L: operand = _regL; break;
        case Opcode::ADD_A_aHL: case Opcode::ADC_A_aHL:
            operand = _memory->read(regHL());
            cycles = 2;
            break;
        case Opcode::ADD_A_N: case Opcode::ADC_A_N:
            operand = _memory->read(_programCounter++);
            cycles = 2;
            break;
    }

    // All ADC instructions are at or above 0x08 in their row.
    if (((opcode & 0x0f) >= 0x08) && cFlag()) {
        operand++;
    }

    const uint8_t result = _regA + operand;

    zFlag(result == 0);
    nFlag(false);
    hFlag((((_regA & 0xf) + (operand & 0xf)) & 0x10) != 0);
    cFlag(result < _regA || result < operand);

    _regA = result;

    return cycles;
}

int8_t CPU::I_8BitSubtract(uint8_t opcode) {
    uint8_t operand = 0;
    int8_t cycles = 1;

    switch(opcode) {
        case Opcode::SUB_A: case Opcode::SBC_A_A: operand = _regA; break;
        case Opcode::SUB_B: case Opcode::SBC_A_B: operand = _regB; break;
        case Opcode::SUB_C: case Opcode::SBC_A_C: operand = _regC; break;
        case Opcode::SUB_D: case Opcode::SBC_A_D: operand = _regD; break;
        case Opcode::SUB_E: case Opcode::SBC_A_E: operand = _regE; break;
        case Opcode::SUB_H: case Opcode::SBC_A_H: operand = _regH; break;
        case Opcode::SUB_L: case Opcode::SBC_A_L: operand = _regL; break;
        case Opcode::SUB_aHL: case Opcode::SBC_A_aHL:
            operand = _memory->read(regHL());
            cycles = 2;
            break;
        case Opcode::SUB_N: case Opcode::SBC_A_N:
            operand = _memory->read(_programCounter++);
            cycles = 2;
            break;
    }

    uint8_t carryValue = 0;

    // All SBC instructions are at or above 0x08 in their row.
    if (((opcode & 0x0f) >= 0x08) && cFlag()) {
        carryValue = 1;
    }

    const uint8_t result = _regA - operand - carryValue;

    zFlag(result == 0);
    nFlag(true);
    hFlag(((_regA & 0x0f) - (operand & 0x0f) - carryValue) < 0);
    cFlag(result > _regA || result > operand);

    _regA = result;

    return cycles;
}

#define DECODE_LOGICAL_OPERAND(operation) \
    uint8_t operand = 0; \
    int8_t cycles = 1; \
 \
    switch(opcode) { \
        case Opcode::operation##_A: operand = _regA; break; \
        case Opcode::operation##_B: operand = _regB; break; \
        case Opcode::operation##_C: operand = _regC; break; \
        case Opcode::operation##_D: operand = _regD; break; \
        case Opcode::operation##_E: operand = _regE; break; \
        case Opcode::operation##_H: operand = _regH; break; \
        case Opcode::operation##_L: operand = _regL; break; \
        case Opcode::operation##_aHL: \
            operand = _memory->read(regHL()); \
            cycles = 2; \
            break; \
        case Opcode::operation##_N: \
            operand = _memory->read(_programCounter++); \
            cycles = 2; \
            break; \
    } \

int8_t CPU::I_And(uint8_t opcode) {
    DECODE_LOGICAL_OPERAND(AND);

    _regA = _regA & operand;

    zFlag(_regA == 0);
    nFlag(false);
    hFlag(true);
    cFlag(false);

    return cycles;
}

int8_t CPU::I_Or(uint8_t opcode) {
    DECODE_LOGICAL_OPERAND(OR);

    _regA = _regA | operand;

    zFlag(_regA == 0);
    nFlag(false);
    hFlag(false);
    cFlag(false);

    return cycles;
}

int8_t CPU::I_Xor(uint8_t opcode) {
    DECODE_LOGICAL_OPERAND(XOR);

    _regA = _regA ^ operand;

    zFlag(_regA == 0);
    nFlag(false);
    hFlag(false);
    cFlag(false);

    return cycles;
}
