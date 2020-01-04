#include "TestBase.H"

#include "Opcodes.h"

#include <iostream>

// Combo Registers ////////////////////////////////////////////////////////////
TEST_CASE("read BC") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    testCPU._regB = 0x01;
    testCPU._regC = 0x02;
    auto read = testCPU.regBC();
    CHECK(read == 0x0102);

    testCPU._regB = 0x03;
    testCPU._regC = 0x04;
    read = testCPU.regBC();
    CHECK(read == 0x0304);
}

TEST_CASE("read DE") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    testCPU._regD = 0x05;
    testCPU._regE = 0x06;
    auto read = testCPU.regDE();
    CHECK(read == 0x0506);

    testCPU._regD = 0x07;
    testCPU._regE = 0x08;
    read = testCPU.regDE();
    CHECK(read == 0x0708);
}

TEST_CASE("read HL") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    testCPU._regH = 0x09;
    testCPU._regL = 0x0A;
    auto read = testCPU.regHL();
    CHECK(read == 0x090A);

    testCPU._regH = 0x0B;
    testCPU._regL = 0x0C;
    read = testCPU.regHL();
    CHECK(read == 0x0B0C);
}

TEST_CASE("write BC") {
    WITH_CPU_AND_SIMPLE_MEMORY();
    testCPU._regB = 0;
    testCPU._regC = 0;

    testCPU.regBC(0x0102);
    CHECK(testCPU._regB == 0x01);
    CHECK(testCPU._regC == 0x02);
}

TEST_CASE("write DE") {
    WITH_CPU_AND_SIMPLE_MEMORY();
    testCPU._regD = 0;
    testCPU._regE = 0;

    testCPU.regDE(0x0304);
    CHECK(testCPU._regD == 0x03);
    CHECK(testCPU._regE == 0x04);
}

TEST_CASE("write HL") {
    WITH_CPU_AND_SIMPLE_MEMORY();
    testCPU._regH = 0;
    testCPU._regL = 0;

    testCPU.regHL(0x0506);
    CHECK(testCPU._regH == 0x05);
    CHECK(testCPU._regL == 0x06);
}

// Flags //////////////////////////////////////////////////////////////////////

TEST_CASE("rw Z") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // Set everything
    testCPU._flags = 0xff;
    testCPU.zFlag(false);
    CHECK(testCPU._flags == 0x7f);
    CHECK(testCPU.zFlag() == false);
    testCPU.zFlag(true);
    CHECK(testCPU._flags == 0xff);
    CHECK(testCPU.zFlag() == true);
}

TEST_CASE("rw N") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // Set everything
    testCPU._flags = 0xff;
    testCPU.nFlag(false);
    CHECK(testCPU._flags == 0xbf);
    CHECK(testCPU.nFlag() == false);
    testCPU.nFlag(true);
    CHECK(testCPU._flags == 0xff);
    CHECK(testCPU.nFlag() == true);
}

TEST_CASE("rw H") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // Set everything
    testCPU._flags = 0xff;
    testCPU.hFlag(false);
    CHECK(testCPU._flags == 0xdf);
    CHECK(testCPU.hFlag() == false);
    testCPU.hFlag(true);
    CHECK(testCPU._flags == 0xff);
    CHECK(testCPU.hFlag() == true);
}

TEST_CASE("rw C") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // Set everything
    testCPU._flags = 0xff;
    testCPU.cFlag(false);
    CHECK(testCPU._flags == 0xef);
    CHECK(testCPU.cFlag() == false);
    testCPU.cFlag(true);
    CHECK(testCPU._flags == 0xff);
    CHECK(testCPU.cFlag() == true);
}

// Instructions ///////////////////////////////////////////////////////////////

TEST_CASE("load immediate") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // This program sets A, through L to 1, through 7.
    // No flags should be impacted.
    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_A_n,
        0x01,
        Opcode::LD_B_n,
        0x02,
        Opcode::LD_C_n,
        0x03,
        Opcode::LD_D_n,
        0x04,
        Opcode::LD_E_n,
        0x05,
        Opcode::LD_H_n,
        0x06,
        Opcode::LD_L_n,
        0x07,
    });

    // Zero out the registers and flags to get to a known state.
    testCPU._flags = 0;
    testCPU._regA = 0;
    testCPU._regB = 0;
    testCPU._regC = 0;
    testCPU._regD = 0;
    testCPU._regE = 0;
    testCPU._regH = 0;
    testCPU._regL = 0;

    // Each instruction takes 8 clock cycles (2 machine cycles). 56 clocks
    // should be exactly enough to execute the program.

    CLOCK(56);

    CHECK(testCPU._regA == 0x01);
    CHECK(testCPU._regB == 0x02);
    CHECK(testCPU._regC == 0x03);
    CHECK(testCPU._regD == 0x04);
    CHECK(testCPU._regE == 0x05);
    CHECK(testCPU._regH == 0x06);
    CHECK(testCPU._regL == 0x07);

    CHECK(testCPU._flags == 0);

    CHECK(testCPU._programCounter == INIT_VECTOR + 14);
}

#define REGISTER_STORAGE_TEST(reg, idx) \
    testCPU.reset(); \
    \
    testCPU._regA = 0x00; \
    testCPU._regB = 0x01; \
    testCPU._regC = 0x02; \
    testCPU._regD = 0x03; \
    testCPU._regE = 0x04; \
    testCPU._regH = 0x05; \
    testCPU._regL = 0x06; \
    \
    simpleMemory->write(INIT_VECTOR, { \
        Opcode::LD_##reg##_A, \
        Opcode::LD_##reg##_B, \
        Opcode::LD_##reg##_C, \
        Opcode::LD_##reg##_D, \
        Opcode::LD_##reg##_E, \
        Opcode::LD_##reg##_H, \
        Opcode::LD_##reg##_L, \
    }); \
    \
    for (int i = 0; i < 7; i++) { \
        /* All of these take 1 machine cycle */ \
        CLOCK(4); \
        if (i != idx) { \
            CHECK(testCPU._reg##reg == (uint8_t)i); \
            CHECK(testCPU._programCounter == INIT_VECTOR + i + 1); \
        } \
    } \

TEST_CASE("transfer register") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    REGISTER_STORAGE_TEST(A, 0);
    REGISTER_STORAGE_TEST(B, 1);
    REGISTER_STORAGE_TEST(C, 2);
    REGISTER_STORAGE_TEST(D, 3);
    REGISTER_STORAGE_TEST(E, 4);
    REGISTER_STORAGE_TEST(H, 5);
    REGISTER_STORAGE_TEST(L, 6);
}

TEST_CASE("transfter HL to SP") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_HL_SP,
    });

    testCPU._stackPointer = 0x0000;
    testCPU.regHL(0x1234);

    CLOCK(8);
    CHECK(testCPU._stackPointer == 0x1234);
}

TEST_CASE("load HL") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_A_aHL,
        Opcode::LD_B_aHL,
        Opcode::LD_C_aHL,
        Opcode::LD_D_aHL,
        Opcode::LD_E_aHL,
        Opcode::LD_H_aHL,
        Opcode::LD_L_aHL,
    });

    simpleMemory->write(0x00, 0xAA);

    testCPU._regA = 0x00;
    testCPU._regB = 0x00;
    testCPU._regC = 0x00;
    testCPU._regD = 0x00;
    testCPU._regE = 0x00;
    testCPU._regH = 0x00;
    testCPU._regL = 0x00;

    // Run for A-H
    CLOCK(48);
    CHECK(testCPU._regA == 0xAA);
    CHECK(testCPU._regB == 0xAA);
    CHECK(testCPU._regC == 0xAA);
    CHECK(testCPU._regD == 0xAA);
    CHECK(testCPU._regE == 0xAA);
    CHECK(testCPU._regH == 0xAA);

    // Reset H so the address is correct
    testCPU._regH = 0x00;
    CLOCK(8);
    CHECK(testCPU._regL == 0xAA);
}

TEST_CASE("load address to A") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0x00, {
        0x01,
        0x02,
        0x03,
    });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_A_aBC,
        Opcode::LD_A_aDE,
        Opcode::LD_A_aNN,
        0x02,
        0x00,
    });

    testCPU.regBC(0x0000);
    testCPU.regDE(0x0001);

    CLOCK(8);
    CHECK(testCPU._regA == 0x01);

    CLOCK(8);
    CHECK(testCPU._regA == 0x02);

    CLOCK(16);
    CHECK(testCPU._regA == 0x03);
}

TEST_CASE("store register to HL address") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    const auto testAddress = 0x55aa;
    simpleMemory->write(testAddress, 0xff);

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_aHL_A,
        Opcode::LD_aHL_B,
        Opcode::LD_aHL_C,
        Opcode::LD_aHL_D,
        Opcode::LD_aHL_E,
    });

    testCPU._regA = 0x00;
    testCPU._regB = 0x01;
    testCPU._regC = 0x02;
    testCPU._regD = 0x03;
    testCPU._regE = 0x04;

    testCPU.regHL(testAddress);

    for (int i = 0; i < 5; i++) {
        CLOCK(8);
        CHECK(simpleMemory->read(testAddress) == (uint8_t)i);
    }
}

TEST_CASE("store literal to HL address") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    const auto testAddress = 0x55aa;
    simpleMemory->write(testAddress, 0xff);

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_aHL_n,
        0x12,
    });

    testCPU.regHL(testAddress);

    CLOCK(12);

    CHECK(simpleMemory->read(testAddress) == 0x12);
}

TEST_CASE("store A to address") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_aBC_A,
        Opcode::LD_aDE_A,
        Opcode::LD_aNN_A,
        0x02,
        0x00,
    });

    simpleMemory->write(0x0000, { 0x00, 0x00, 0x00 });

    testCPU.regBC(0x0000);
    testCPU.regDE(0x0001);

    testCPU._regA = 0x01;
    CLOCK(8);
    testCPU._regA = 0x02;
    CLOCK(8);
    testCPU._regA = 0x03;
    CLOCK(16);

    CHECK(simpleMemory->read(0x0000) == 0x01);
    CHECK(simpleMemory->read(0x0001) == 0x02);
    CHECK(simpleMemory->read(0x0002) == 0x03);
}

TEST_CASE("load A indirect from C") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0xff00, { 0x00, 0x01, 0x02, 0x03 });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_A_afC,
        Opcode::LD_A_afC,
        Opcode::LD_A_afC,
        Opcode::LD_A_afC,
    });

    for (int i = 0; i < 4; i++) {
        testCPU._regC = (uint8_t)i;
        CLOCK(8);
        CHECK(testCPU._regA == (uint8_t)i);
    }
}

TEST_CASE("store A indirect via C") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0xff00, { 0xff, 0xff, 0xff, 0xff });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_afC_A,
        Opcode::LD_afC_A,
        Opcode::LD_afC_A,
        Opcode::LD_afC_A,
    });

    for (int i = 0; i < 4; i++) {
        testCPU._regC = (uint8_t)i;
        testCPU._regA = (uint8_t)i;
        CLOCK(8);
        CHECK(simpleMemory->read(0xff00 + (uint16_t)i) == (uint8_t)i);
    }
}

TEST_CASE("decremental HL load") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0x00, {
        0x03,
        0x02,
        0x01,
        0x00,
    });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LDD_A_aHL,
        Opcode::LDD_A_aHL,
        Opcode::LDD_A_aHL,
        Opcode::LDD_A_aHL,
    });

    testCPU.regHL(0x03);

    for (int i = 0; i < 4; i++) {
        CLOCK(8);
        CHECK(testCPU._regA == (uint8_t)i);
    }
}

TEST_CASE("incremental HL load") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0x00, {
        0x00,
        0x01,
        0x02,
        0x03,
    });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LDI_A_aHL,
        Opcode::LDI_A_aHL,
        Opcode::LDI_A_aHL,
        Opcode::LDI_A_aHL,
    });

    testCPU.regHL(0x00);

    for (int i = 0; i < 4; i++) {
        CLOCK(8);
        CHECK(testCPU._regA == (uint8_t)i);
    }
}

TEST_CASE("decremental HL store") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0x00, { 0xff, 0xff, 0xff, 0xff });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LDD_aHL_A,
        Opcode::LDD_aHL_A,
        Opcode::LDD_aHL_A,
        Opcode::LDD_aHL_A,
    });

    testCPU._regA = 0xaa;
    testCPU.regHL(0x03);

    CLOCK(32);

    CHECK(testCPU.regHL() == 0xffff);

    for (int i = 0; i < 4; i++) {
        CHECK(simpleMemory->read((uint16_t)i) == 0xaa);
    }
}

TEST_CASE("incremental HL store") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0x00, { 0xff, 0xff, 0xff, 0xff });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LDI_aHL_A,
        Opcode::LDI_aHL_A,
        Opcode::LDI_aHL_A,
        Opcode::LDI_aHL_A,
    });

    testCPU._regA = 0xaa;
    testCPU.regHL(0x00);

    CLOCK(32);

    CHECK(testCPU.regHL() == 0x04);

    for (int i = 0; i < 4; i++) {
        CHECK(simpleMemory->read((uint16_t)i) == 0xaa);
    }
}

TEST_CASE("load n + ff00") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0xff00, { 0x00, 0x01, 0x02, 0x03 });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LDH_A_afN,
        0x00,
        Opcode::LDH_A_afN,
        0x01,
        Opcode::LDH_A_afN,
        0x02,
        Opcode::LDH_A_afN,
        0x03,
    });

    for (int i = 0; i < 4; i++) {
        CLOCK(12);
        CHECK(testCPU._regA == (uint8_t)i);
    }
}

TEST_CASE("store n + ff00") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(0xff00, { 0xff, 0xff, 0xff, 0xff });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LDH_afN_A,
        0x00,
        Opcode::LDH_afN_A,
        0x01,
        Opcode::LDH_afN_A,
        0x02,
        Opcode::LDH_afN_A,
        0x03,
    });

    testCPU._regA = 0xaa;
    CLOCK(48);

    for (int i = 0; i < 4; i++) {
        CHECK(simpleMemory->read(0xff00 + (uint16_t)i) == 0xaa);
    }
}

TEST_CASE("load immediate 16-bit") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_BC_NN,
        0x02,
        0x01,
        Opcode::LD_DE_NN,
        0x04,
        0x03,
        Opcode::LD_HL_NN,
        0x06,
        0x05,
        Opcode::LD_SP_NN,
        0x08,
        0x07,
    });

    testCPU.regBC(0x0000);
    testCPU.regDE(0x0000);
    testCPU.regHL(0x0000);
    testCPU._stackPointer = 0x0000;

    CLOCK(48);

    CHECK(testCPU.regBC() == 0x0102);
    CHECK(testCPU.regDE() == 0x0304);
    CHECK(testCPU.regHL() == 0x0506);
    CHECK(testCPU._stackPointer == 0x0708);
}

TEST_CASE("load HL with value from stack pointer + n") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // NOTE: Documentation on how flags work for this instruction is rough.
    //       Flag logic assumes reference on StackOverflow is correct.

    // 16-bit loads will get these little-endian.
    simpleMemory->write(0x00, { 0x00, 0x01, 0x02, 0x03, 0x04 });

    int8_t offsetMinus1 = -1;
    uint8_t rawOffsetMinus1 = *reinterpret_cast<uint8_t*>(&offsetMinus1);

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_HL_aSPN,
        0x01,
        Opcode::LD_HL_aSPN,
        rawOffsetMinus1,
    });

    // Part 1: SP + 1
    testCPU._stackPointer = 0x0001;
    testCPU.regHL(0x0000);
    CLOCK(12);
    CHECK(testCPU.regHL() == 0x0302);
    CHECK(testCPU._flags == 0x00);

    // Part 2: SP - 1
    testCPU._stackPointer = 0x0001;
    testCPU.regHL(0x0000);
    CLOCK(12);
    CHECK(testCPU.regHL() == 0x0100);
    CHECK(testCPU.zFlag() == false);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == true);
    CHECK(testCPU.cFlag() == true);
}

TEST_CASE("store stack pointer") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    testCPU._stackPointer = 0x55aa;

    simpleMemory->write(0x1122, 0x00);
    simpleMemory->write(0x1123, 0x00);

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_aNN_SP,
        0x22, // Little-endian!
        0x11,
    });

    CLOCK(20);

    CHECK(simpleMemory->read(0x1122) == 0xaa); // Little-endian!
    CHECK(simpleMemory->read(0x1123) == 0x55);
}

TEST_CASE("push register") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    testCPU.regAF(0x0102);
    testCPU.regBC(0x0304);
    testCPU.regDE(0x0506);
    testCPU.regHL(0x0708);

    simpleMemory->write(INIT_STACK_POINTER - 6, {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::PUSH_AF,
        Opcode::PUSH_BC,
        Opcode::PUSH_DE,
        Opcode::PUSH_HL,
    });

    CLOCK(64);

    CHECK(testCPU._stackPointer == INIT_STACK_POINTER - 8);
    CHECK(simpleMemory->readLI(INIT_STACK_POINTER - 2) == 0x0102);
    CHECK(simpleMemory->readLI(INIT_STACK_POINTER - 4) == 0x0304);
    CHECK(simpleMemory->readLI(INIT_STACK_POINTER - 6) == 0x0506);
    CHECK(simpleMemory->readLI(INIT_STACK_POINTER - 8) == 0x0708);
}

TEST_CASE("pop register") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_STACK_POINTER - 8, {
        0x08,
        0x07,
        0x06,
        0x05,
        0x04,
        0x03,
        0x02,
        0x01,
    });

    simpleMemory->write(INIT_VECTOR, {
        Opcode::POP_HL,
        Opcode::POP_DE,
        Opcode::POP_BC,
        Opcode::POP_AF,
    });

    testCPU.regAF(0x0000);
    testCPU.regBC(0x0000);
    testCPU.regDE(0x0000);
    testCPU.regHL(0x0000);

    testCPU._stackPointer = INIT_STACK_POINTER - 8;

    CLOCK(48);

    CHECK(testCPU._stackPointer == INIT_STACK_POINTER);
    CHECK(testCPU.regAF() == 0x0102);
    CHECK(testCPU.regBC() == 0x0304);
    CHECK(testCPU.regDE() == 0x0506);
    CHECK(testCPU.regHL() == 0x0708);
}

TEST_CASE("push/pop") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    // arbitrary value
    testCPU.regBC(0x1234);

    // Accumulator and flags
    testCPU._regA = 0xff;
    testCPU._flags = 0x00;
    testCPU.zFlag(true);
    testCPU.cFlag(true);

    simpleMemory->write(INIT_VECTOR, {
        Opcode::PUSH_BC,
        Opcode::PUSH_AF,
        Opcode::POP_AF,
        Opcode::POP_BC,
    });

    CLOCK(32);

    // Values should be saved, so wipe them out on the CPU
    testCPU.regBC(0x0000);
    testCPU._regA = 0x00;
    testCPU._flags = 0x00;

    CLOCK(24);

    CHECK(testCPU.regBC() == 0x1234);
    CHECK(testCPU._regA == 0xff);
    CHECK(testCPU.zFlag() == true);
    CHECK(testCPU.cFlag() == true);
    CHECK(testCPU.hFlag() == false);
    CHECK(testCPU.nFlag() == false);
}

TEST_CASE("add") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::ADD_A_A,
        Opcode::ADD_A_B,
        Opcode::ADD_A_C,
        Opcode::ADD_A_D,
        Opcode::ADD_A_E,
        Opcode::ADD_A_H,
        Opcode::ADD_A_L,
        Opcode::ADD_A_aHL,
        Opcode::ADD_A_N,
        0x09,
    });

    testCPU._regA = 1;
    testCPU._regB = 2;
    testCPU._regC = 3;
    testCPU._regD = 4;
    testCPU._regE = 5;
    testCPU._regH = 6;
    testCPU._regL = 7;
    simpleMemory->write(0x0607, 0x08);

    CLOCK(44);

    CHECK(testCPU._regA == (uint8_t)46);
}

TEST_CASE("add and check flags") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::ADD_A_A,
        Opcode::ADD_A_B,
        Opcode::ADD_A_B,
        Opcode::ADD_A_B,
    });

    // Part 1 - Zero Flag
    testCPU._regA = 0;
    CLOCK(4);

    CHECK(testCPU._regA == 0);
    CHECK(testCPU.zFlag() == true);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == false);
    CHECK(testCPU.cFlag() == false);

    // Part 2 - Half-Carry Flag
    testCPU._regA = 0x0f;
    testCPU._regB = 0x01;
    CLOCK(4);

    CHECK(testCPU._regA == 0x10);
    CHECK(testCPU.zFlag() == false);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == true);
    CHECK(testCPU.cFlag() == false);

    // Part 3 - Carry Flag (1)
    testCPU._regA = 0xff;
    testCPU._regB = 0x01;
    CLOCK(4);

    CHECK(testCPU._regA == 0x00);
    CHECK(testCPU.zFlag() == true);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == true);
    CHECK(testCPU.cFlag() == true);

    // Part 3 - Carry Flag (2)
    testCPU._regA = 0xff;
    testCPU._regB = 0x02;
    CLOCK(4);

    CHECK(testCPU._regA == 0x01);
    CHECK(testCPU.zFlag() == false);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == true);
    CHECK(testCPU.cFlag() == true);
}

TEST_CASE("add with carry") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::ADD_A_B,
        Opcode::ADC_A_C,
        Opcode::ADC_A_D,
    });

    testCPU._regA = 0xff;
    testCPU._regB = 0x02; // Cause a carry, 0xff -> 0x01
    testCPU._regC = 0x01; // 0x01 + 0x01 + carry
    testCPU._regD = 0x01; // Carry will be clear by now, so 0x03 + 0x01 + 0

    CLOCK(8);
    CHECK(testCPU._regA == 0x03);
    CLOCK(4);
    CHECK(testCPU._regA == 0x04);
}

TEST_CASE("subtract") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::SUB_B,
        Opcode::SUB_C,
        Opcode::SUB_D,
        Opcode::SUB_E,
        Opcode::SUB_H,
        Opcode::SUB_L,
        Opcode::SUB_aHL,
        Opcode::SUB_N,
        0x09,
    });

    testCPU._regA = 45;
    testCPU._regB = 2;
    testCPU._regC = 3;
    testCPU._regD = 4;
    testCPU._regE = 5;
    testCPU._regH = 6;
    testCPU._regL = 7;
    simpleMemory->write(0x0607, 0x08);

    CLOCK(40);

    CHECK(testCPU._regA == (uint8_t)1);
}

TEST_CASE("and") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::AND_A,
        Opcode::AND_B,
        Opcode::AND_C,
        Opcode::AND_D,
        Opcode::AND_E,
        Opcode::AND_H,
        Opcode::AND_L,
        Opcode::AND_aHL,
        Opcode::AND_N,
        0x00,
    });

    testCPU._regA = 0xff;
    testCPU._regB = 0x7f;
    testCPU._regC = 0x3f;
    testCPU._regD = 0x1f;
    testCPU._regE = 0x0f;
    testCPU._regH = 0x07;
    testCPU._regL = 0x03;
    simpleMemory->write(0x0703, 0x01);

    CLOCK(4);
    CHECK(testCPU._regA == 0xff);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0x7f);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0x3f);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0x1f);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0x0f);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0x07);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0x03);
    CHECK(testCPU.zFlag() == false);

    CLOCK(8);
    CHECK(testCPU._regA == 0x01);
    CHECK(testCPU.zFlag() == false);

    CLOCK(8);
    CHECK(testCPU._regA == 0x00);
    CHECK(testCPU.zFlag() == true);
}

TEST_CASE("or") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::OR_A,
        Opcode::OR_B,
        Opcode::OR_C,
        Opcode::OR_D,
        Opcode::OR_E,
        Opcode::OR_H,
        Opcode::OR_L,
        Opcode::OR_aHL,
        Opcode::OR_N,
        0x01,
    });

    testCPU._regA = 0x00;
    testCPU._regB = 0x80;
    testCPU._regC = 0x40;
    testCPU._regD = 0x20;
    testCPU._regE = 0x10;
    testCPU._regH = 0x08;
    testCPU._regL = 0x04;
    simpleMemory->write(0x0804, 0x02);

    CLOCK(4);
    CHECK(testCPU._regA == 0x00);
    CHECK(testCPU.zFlag() == true);

    CLOCK(4);
    CHECK(testCPU._regA == 0x80);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xc0);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xe0);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xf0);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xf8);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xfc);
    CHECK(testCPU.zFlag() == false);

    CLOCK(8);
    CHECK(testCPU._regA == 0xfe);
    CHECK(testCPU.zFlag() == false);

    CLOCK(8);
    CHECK(testCPU._regA == 0xff);
    CHECK(testCPU.zFlag() == false);
}

TEST_CASE("xor") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::XOR_A,
        Opcode::XOR_B,
        Opcode::XOR_C,
        Opcode::XOR_D,
        Opcode::XOR_E,
        Opcode::XOR_H,
        Opcode::XOR_L,
        Opcode::XOR_aHL,
        Opcode::XOR_N,
        0xff,
    });

    testCPU._regA = 0x00;
    testCPU._regB = 0x80;
    testCPU._regC = 0x40;
    testCPU._regD = 0x20;
    testCPU._regE = 0x10;
    testCPU._regH = 0x08;
    testCPU._regL = 0x04;
    simpleMemory->write(0x0804, 0x02);

    CLOCK(4);
    CHECK(testCPU._regA == 0x00);
    CHECK(testCPU.zFlag() == true);

    CLOCK(4);
    CHECK(testCPU._regA == 0x80);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xc0);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xe0);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xf0);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xf8);
    CHECK(testCPU.zFlag() == false);

    CLOCK(4);
    CHECK(testCPU._regA == 0xfc);
    CHECK(testCPU.zFlag() == false);

    CLOCK(8);
    CHECK(testCPU._regA == 0xfe);
    CHECK(testCPU.zFlag() == false);

    CLOCK(8);
    CHECK(testCPU._regA == 0x01);
    CHECK(testCPU.zFlag() == false);
}

TEST_CASE("increment") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::INC_A,
        Opcode::INC_B,
        Opcode::INC_C,
        Opcode::INC_D,
        Opcode::INC_E,
        Opcode::INC_H,
        Opcode::INC_L,
        Opcode::INC_aHL,
    });

    testCPU._regA = 0x00;
    testCPU._regB = 0x01;
    testCPU._regC = 0x02;
    testCPU._regD = 0x03;
    testCPU._regE = 0x04;
    testCPU._regH = 0x05;
    testCPU._regL = 0x06;
    simpleMemory->write(0x0607, 0x07);

    CLOCK(40);

    CHECK(testCPU._regA == 0x01);
    CHECK(testCPU._regB == 0x02);
    CHECK(testCPU._regC == 0x03);
    CHECK(testCPU._regD == 0x04);
    CHECK(testCPU._regE == 0x05);
    CHECK(testCPU._regH == 0x06);
    CHECK(testCPU._regL == 0x07);
    CHECK(simpleMemory->read(0x0607) == 0x08);
}

TEST_CASE("decrement") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::DEC_A,
        Opcode::DEC_B,
        Opcode::DEC_C,
        Opcode::DEC_D,
        Opcode::DEC_E,
        Opcode::DEC_H,
        Opcode::DEC_L,
        Opcode::DEC_aHL,
    });

    testCPU._regA = 0x01;
    testCPU._regB = 0x02;
    testCPU._regC = 0x03;
    testCPU._regD = 0x04;
    testCPU._regE = 0x05;
    testCPU._regH = 0x06;
    testCPU._regL = 0x07;
    simpleMemory->write(0x0506, 0x08);

    CLOCK(40);

    CHECK(testCPU._regA == 0x00);
    CHECK(testCPU._regB == 0x01);
    CHECK(testCPU._regC == 0x02);
    CHECK(testCPU._regD == 0x03);
    CHECK(testCPU._regE == 0x04);
    CHECK(testCPU._regH == 0x05);
    CHECK(testCPU._regL == 0x06);
    CHECK(simpleMemory->read(0x0506) == 0x07);
}

TEST_CASE("16-bit add") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::ADD_HL_BC,
        Opcode::ADD_HL_DE,
        Opcode::ADD_HL_HL,
        Opcode::ADD_HL_SP,
    });

    testCPU.regHL(0x01);
    testCPU.regBC(0x01);

    CLOCK(8);
    CHECK(testCPU.regHL() == 0x02);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == false);
    CHECK(testCPU.cFlag() == false);

    testCPU.regHL(0xff9b);
    testCPU.regDE(0x0069);

    CLOCK(8);
    CHECK(testCPU.regHL() == 0x04);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == true);
    CHECK(testCPU.cFlag() == true);

    testCPU.regHL(0x0a0b);

    CLOCK(8);
    CHECK(testCPU.regHL() == 0x1416);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == false);
    CHECK(testCPU.cFlag() == false);

    testCPU.regHL(0x0ffff);
    testCPU._stackPointer = 0x0001;

    CLOCK(8);
    CHECK(testCPU.regHL() == 0x0000);
    CHECK(testCPU.nFlag() == false);
    CHECK(testCPU.hFlag() == true);
    CHECK(testCPU.cFlag() == true);
}

TEST_CASE("add to SP") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    int8_t rawNeg2 = -2;
    auto neg2 = *reinterpret_cast<uint8_t*>(&rawNeg2);

    simpleMemory->write(INIT_VECTOR, {
        Opcode::ADD_SP_N,
        (int8_t)1,
        Opcode::ADD_SP_N,
        neg2,
    });

    testCPU._stackPointer = 0x05;

    CLOCK(16);
    CHECK(testCPU._stackPointer == 0x06);

    CLOCK(16);
    CHECK(testCPU._stackPointer == 0x04);
}

TEST_CASE("16-bit increment") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::INC_BC,
        Opcode::INC_DE,
        Opcode::INC_HL,
        Opcode::INC_SP,
    });

    testCPU.regBC(0x00);
    testCPU.regDE(0x01);
    testCPU.regHL(0x02);
    testCPU._stackPointer = 0x03;

    CLOCK(32);

    CHECK(testCPU.regBC() == 0x01);
    CHECK(testCPU.regDE() == 0x02);
    CHECK(testCPU.regHL() == 0x03);
    CHECK(testCPU._stackPointer == 0x04);
}

TEST_CASE("16-bit decrement") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::DEC_BC,
        Opcode::DEC_DE,
        Opcode::DEC_HL,
        Opcode::DEC_SP,
    });

    testCPU.regBC(0x00);
    testCPU.regDE(0x01);
    testCPU.regHL(0x02);
    testCPU._stackPointer = 0x03;

    CLOCK(32);

    CHECK(testCPU.regBC() == 0xffff);
    CHECK(testCPU.regDE() == 0x0000);
    CHECK(testCPU.regHL() == 0x0001);
    CHECK(testCPU._stackPointer == 0x0002);
}

TEST_CASE("DAA") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_A_n,
        0x05, // 5 is the same in BCD and hex
        Opcode::ADD_A_A,
        Opcode::DAA,
        Opcode::LD_A_n,
        0x20, // Treat this as BCD 20, not decimal 32
        Opcode::LD_B_n,
        0x01,
        Opcode::SUB_B, // Should put us at 0x1f, but we want 0x19
        Opcode::DAA,
    });

    // 0x05 + 0x05 = 0x0A. Decimal adjust should be BCD 10 (0x10)

    CLOCK(16);
    CHECK(testCPU._regA == 0x10);

    CLOCK(24);
    CHECK(testCPU._regA == 0x19);
}

TEST_CASE("complement A") {
    WITH_CPU_AND_SIMPLE_MEMORY();

    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_A_n,
        0xf0,
        Opcode::CPL,
        Opcode::CPL,
    });

    CLOCK(12);
    CHECK(testCPU._regA == 0x0f);
    CLOCK(4);
    CHECK(testCPU._regA == 0xf0);
}
