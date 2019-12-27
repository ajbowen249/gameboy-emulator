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

    // This program sets B, C, D, E, H, and L to 1, through 6.
    // No flags should be impacted.
    simpleMemory->write(INIT_VECTOR, {
        Opcode::LD_B_n,
        0x01,
        Opcode::LD_C_n,
        0x02,
        Opcode::LD_D_n,
        0x03,
        Opcode::LD_E_n,
        0x04,
        Opcode::LD_H_n,
        0x05,
        Opcode::LD_L_n,
        0x06,
    });

    // Zero out the registers and flags to get to a known state.
    testCPU._flags = 0;
    testCPU._regB = 0;
    testCPU._regC = 0;
    testCPU._regD = 0;
    testCPU._regE = 0;
    testCPU._regH = 0;
    testCPU._regL = 0;

    // Each instruction takes 8 clock cycles (2 machine cycles). 48 clocks
    // should be exactly enough to execute the program up.

    CLOCK(48);

    CHECK(testCPU._regB == 0x01);
    CHECK(testCPU._regC == 0x02);
    CHECK(testCPU._regD == 0x03);
    CHECK(testCPU._regE == 0x04);
    CHECK(testCPU._regH == 0x05);
    CHECK(testCPU._regL == 0x06);

    CHECK(testCPU._programCounter == INIT_VECTOR + 12);
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

TEST_CASE("store register to HL") {
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
