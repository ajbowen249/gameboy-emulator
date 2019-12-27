#include "TestBase.H"

#include "Opcodes.h"

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
    // should be exactly enough to execute the program.

    for (int i = 0; i < 48; i++) {
        testCPU.clock();
    }

    CHECK(testCPU._regB == 0x01);
    CHECK(testCPU._regC == 0x02);
    CHECK(testCPU._regD == 0x03);
    CHECK(testCPU._regE == 0x04);
    CHECK(testCPU._regH == 0x05);
    CHECK(testCPU._regL == 0x06);

    CHECK(testCPU._programCounter == INIT_VECTOR + 12);
}
