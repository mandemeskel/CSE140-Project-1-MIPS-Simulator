#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "updatepc.h"

const int START_PC = 0x00400000;
const int PC_INCREMENT = 0x4;

void testExecuteAddu();
void testExecuteBeq();
void testExecuteBne();
void testExecuteJ();
void testExecuteJr();
void testExecuteJal();
void runTest(DecodedInstr *, int, int);

int main (int argc, char *argv[]) {
    printLine("Running UpdatePC() test suite...");

    testExecuteAddu();
    testExecuteBeq();
    testExecuteBne();
    testExecuteJ();
    testExecuteJr();
    testExecuteJal();

    printLine("Finished running tests.");
}

void testExecuteAddu() {
    printf("test addu $11, $12, $13...");
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.rd = 11,
        .regs.r.shamt = 0,
        .regs.r.funct = ADDU_FUNCT
    };
    int val = 30, expectedPC = START_PC + PC_INCREMENT;

    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);
}

void testExecuteBeq() {
    int expectedPC = START_PC + 0x0000A0DF, val = 0;
    DecodedInstr dInst = {
        .type = I,
        .op = BEQ_OPCODE,
        .regs.i.addr_or_immed = expectedPC
    };

    printf("test beq $11, $12, ADDRESS, where $11 - $12 = 0...");
    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);

    printf("test beq $11, $12, ADDRESS, where $11 - $12 = 1...");
    val = 1;
    expectedPC = START_PC + PC_INCREMENT;
    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);
}

void testExecuteBne() {
    int expectedPC = START_PC + 0x0000A0DF, val = 0;
    DecodedInstr dInst = {
        .type = I,
        .op = BEQ_OPCODE,
        .regs.i.addr_or_immed = expectedPC
    };

    printf("test bne $11, $12, ADDRESS, where $11 - $12 = 1...");
    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);

    printf("test bne $11, $12, ADDRESS, where $11 - $12 = 0...");
    val = 1;
    expectedPC = START_PC + PC_INCREMENT;
    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);
}

void testExecuteJ() {
    int expectedPC = START_PC + 0x0000A0DF, val = 0;
    DecodedInstr dInst = {
        .type = J,
        .op = J_OPCODE,
        .regs.j.target = expectedPC
    };

    printf("test j ADDRESS...");
    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);
}

void testExecuteJr() {
    printf("test jr $11...");
    int expectedPC = START_PC + 0x0000A0DF, val = 0;
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rs = expectedPC,
        .regs.r.shamt = 0,
        .regs.r.funct = JR_FUNCT
    };

    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);
}

void testExecuteJal() {
    int expectedPC = START_PC + 0x0000A0DF, val = 0;
    DecodedInstr dInst = {
        .type = J,
        .op = JAL_OPCODE,
        .regs.j.target = expectedPC
    };

    printf("test jal ADDRESS...");
    mips.pc = START_PC;
    runTest(&dInst, val, expectedPC);

    printf("test jal ADDRESS, verify $ra value...");
    int expectedRaValue = START_PC + PC_INCREMENT,
        raValue = mips.registers[31];
    int passed = testTrue(expectedRaValue, raValue, "UpdatePC() set the wrong $ra value");

    if(passed == TRUE)
        printLine("PASSED");
    else
        printLine("FAILED");
}

void runTest(DecodedInstr * dInst, int val, int expectedPC) {
    UpdatePC(dInst, val);
    int passed = testTrue(expectedPC, mips.pc, "UpdatePC() set the wrong PC value");

    if(passed == TRUE)
        printLine("PASSED");
    else
        printLine("FAILED");
}