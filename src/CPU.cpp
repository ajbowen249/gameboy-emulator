#include "CPU.h"

#include "Log.h"
#include "Opcodes.h"

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
            return I_LoadHLWithSPN(opcode);
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
            const auto addrLow = _memory->read(_programCounter++);
            const auto addrHigh = _memory->read(_programCounter++);
            _regA = _memory->read(TO_16(addrHigh, addrLow));
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
            const auto addrLow = _memory->read(_programCounter++);
            const auto addrHigh = _memory->read(_programCounter++);
            _memory->write(TO_16(addrHigh, addrLow), _regA);
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
    const auto lowByte = _memory->read(_programCounter++);
    const auto highByte = _memory->read(_programCounter++);
    const auto fullValue = TO_16(highByte, lowByte);

    switch(opcode) {
        case Opcode::LD_BC_NN: regBC(fullValue); break;
        case Opcode::LD_DE_NN: regDE(fullValue); break;
        case Opcode::LD_HL_NN: regHL(fullValue); break;
        case Opcode::LD_SP_NN: _stackPointer = fullValue; break;
    }

    return 3;
}

int8_t CPU::I_LoadHLWithSPN(uint8_t opcode) {
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
