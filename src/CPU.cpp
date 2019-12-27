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
        case Opcode::LD_B_B:
        case Opcode::LD_B_C:
        case Opcode::LD_B_D:
        case Opcode::LD_B_E:
        case Opcode::LD_B_H:
        case Opcode::LD_B_L:
        case Opcode::LD_C_B:
        case Opcode::LD_C_C:
        case Opcode::LD_C_D:
        case Opcode::LD_C_E:
        case Opcode::LD_C_H:
        case Opcode::LD_C_L:
        case Opcode::LD_D_B:
        case Opcode::LD_D_C:
        case Opcode::LD_D_D:
        case Opcode::LD_D_E:
        case Opcode::LD_D_H:
        case Opcode::LD_D_L:
        case Opcode::LD_E_B:
        case Opcode::LD_E_C:
        case Opcode::LD_E_D:
        case Opcode::LD_E_E:
        case Opcode::LD_E_H:
        case Opcode::LD_E_L:
        case Opcode::LD_H_B:
        case Opcode::LD_H_C:
        case Opcode::LD_H_D:
        case Opcode::LD_H_E:
        case Opcode::LD_H_H:
        case Opcode::LD_H_L:
        case Opcode::LD_L_B:
        case Opcode::LD_L_C:
        case Opcode::LD_L_D:
        case Opcode::LD_L_E:
        case Opcode::LD_L_H:
        case Opcode::LD_L_L:
            return I_TransferRegister(opcode);
    }
}

int8_t CPU::I_LoadImmediate(uint8_t opcode) {
    uint8_t value = _memory->read(_programCounter++);

    switch(opcode) {
        case Opcode::LD_B_n: _regB = value; break;
        case Opcode::LD_C_n: _regC = value; break;
        case Opcode::LD_D_n: _regD = value; break;
        case Opcode::LD_E_n: _regE = value; break;
        case Opcode::LD_H_n: _regH = value; break;
        case Opcode::LD_L_n: _regL = value; break;
    }

    return  2;
}

int8_t CPU::I_TransferRegister(uint8_t opcode) {
    switch(opcode) {
        case Opcode::LD_A_A: _regA = _regA; break;
        case Opcode::LD_A_B: _regA = _regB; break;
        case Opcode::LD_A_C: _regA = _regC; break;
        case Opcode::LD_A_D: _regA = _regD; break;
        case Opcode::LD_A_E: _regA = _regE; break;
        case Opcode::LD_A_H: _regA = _regH; break;
        case Opcode::LD_A_L: _regA = _regL; break;
        case Opcode::LD_B_B: _regB = _regB; break;

        case Opcode::LD_B_C: _regB = _regC; break;
        case Opcode::LD_B_D: _regB = _regD; break;
        case Opcode::LD_B_E: _regB = _regE; break;
        case Opcode::LD_B_H: _regB = _regH; break;
        case Opcode::LD_B_L: _regB = _regL; break;

        case Opcode::LD_C_B: _regC = _regB; break;
        case Opcode::LD_C_C: _regC = _regC; break;
        case Opcode::LD_C_D: _regC = _regD; break;
        case Opcode::LD_C_E: _regC = _regE; break;
        case Opcode::LD_C_H: _regC = _regH; break;
        case Opcode::LD_C_L: _regC = _regL; break;

        case Opcode::LD_D_B: _regD = _regB; break;
        case Opcode::LD_D_C: _regD = _regC; break;
        case Opcode::LD_D_D: _regD = _regD; break;
        case Opcode::LD_D_E: _regD = _regE; break;
        case Opcode::LD_D_H: _regD = _regH; break;
        case Opcode::LD_D_L: _regD = _regL; break;

        case Opcode::LD_E_B: _regE = _regB; break;
        case Opcode::LD_E_C: _regE = _regC; break;
        case Opcode::LD_E_D: _regE = _regD; break;
        case Opcode::LD_E_E: _regE = _regE; break;
        case Opcode::LD_E_H: _regE = _regH; break;
        case Opcode::LD_E_L: _regE = _regL; break;

        case Opcode::LD_H_B: _regH = _regB; break;
        case Opcode::LD_H_C: _regH = _regC; break;
        case Opcode::LD_H_D: _regH = _regD; break;
        case Opcode::LD_H_E: _regH = _regE; break;
        case Opcode::LD_H_H: _regH = _regH; break;
        case Opcode::LD_H_L: _regH = _regL; break;

        case Opcode::LD_L_B: _regL = _regB; break;
        case Opcode::LD_L_C: _regL = _regC; break;
        case Opcode::LD_L_D: _regL = _regD; break;
        case Opcode::LD_L_E: _regL = _regE; break;
        case Opcode::LD_L_H: _regL = _regH; break;
        case Opcode::LD_L_L: _regL = _regL; break;
    }

    return 1;
}
