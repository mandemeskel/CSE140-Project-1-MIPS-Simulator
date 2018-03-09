#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "computer.h"

void testExecuteAdd();
void testExecuteSub();
void testExecuteSll();
void testExecuteSrl();
void testExecuteAnd();
void testExecuteOr();
void testExecuteLui();
void testExecuteSlt();
void testExecuteSw();
void testExecuteLw();
void runTest(DecodedInstr, int, int);

int main (int argc, char *argv[]) {
    printLine("Running RegWrite() test suite...");

    testExecuteAdd();
    testExecuteSub();
    testExecuteSll();
    testExecuteSrl();
    testExecuteAnd();
    testExecuteOr();
    testExecuteLui();
    testExecuteSlt();
    testExecuteSw();
    testExecuteLw();
    
    printLine("Finished running tests.");
}

void testExecuteAdd() {
    printLine("test addu $11, $12, $13: 4 + 5...");
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.rd = 11,
        .regs.r.shamt = 0,
        .regs.r.funct = ADDU_FUNCT
    };
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue + rtValue;

    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;
    mips.registers[dInst.regs.r.rd] = 0;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteSub() {
    printLine("test subu $11, $12, $13: 4 - 5...");
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = 0,
        .regs.r.funct = SUBU_FUNCT
    };
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue - rtValue;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteSll() {
    printLine("test sll $11, $13, 4: 5 << 4...");
    int shamt = 4, rtValues = 5;
    int expectedVal = rtValues << shamt;
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = shamt,
        .regs.r.funct = SLL_FUNCT
    };

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = 0;
    mips.registers[dInst.regs.r.rt] = rtValues;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteSrl() {
    printLine("test srl $11, $13, 4: 80 >> 4...");
    int shamt = 4, rtValues = 80;
    int expectedVal = rtValues >> shamt;
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = shamt,
        .regs.r.funct = SRL_FUNCT
    };

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = 0;
    mips.registers[dInst.regs.r.rt] = rtValues;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteAnd() {
    printLine("test and $11, $12, $13: 4 & 5...");
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue & rtValue;
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = 0,
        .regs.r.funct = AND_FUNCT
    };

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteOr() {
    printLine("test or $11, $12, $13: 4 | 5...");
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue | rtValue;
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = 0,
        .regs.r.funct = OR_FUNCT
    };

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteLui() {
    printLine("test lui $11, 4: 4 << 16...");
    int imm = 4;
    int expectedVal = imm << 16;
    DecodedInstr dInst = {
        .type = I,
        .op = LUI_OPCODE,
        .regs.i.rt = 11,
        .regs.i.addr_or_immed = imm
    };

    runTest(dInst, expectedVal, dInst.regs.r.rt);
}

void testExecuteSlt() {
    printLine("test slt $11, $12, $13: 4 < 5 ? 1 : 0...");
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue < rtValue;
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = 0,
        .regs.r.funct = SLT_FUNCT
    };

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(dInst, expectedVal, dInst.regs.r.rd);

    printLine("test slt $11, $12, $13: 4 < 3 ? 1 : 0...");
    
    rtValue = 3;
    expectedVal = rsValue < rtValue;
    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(dInst, expectedVal, dInst.regs.r.rd);
}

void testExecuteSw() {
    printLine("test sw $11 4($12)...");
    DecodedInstr dInst = {
        .type = I,
        .op = SW_OPCODE,
        .regs.i.rs = 12,
        .regs.i.rt = 11,
        .regs.i.addr_or_immed = 4
    };
    int expectedVal = 9;
    int expectedReg = -1;

    mips.registers[dInst.regs.i.rs] = 0;
    mips.registers[dInst.regs.i.rt] = expectedVal;

    runTest(dInst, expectedVal, expectedReg);
}

void testExecuteLw() {
    printLine("test lw $11 4($12)...");
    DecodedInstr dInst = {
        .type = I,
        .op = LW_OPCODE,
        .regs.i.rs = 12,
        .regs.i.rt = 11,
        .regs.i.addr_or_immed = 4
    };
    int expectedVal = 9;
    int expectedReg = -1;

    mips.registers[dInst.regs.i.rs] = 0;
    mips.registers[dInst.regs.i.rt] = expectedVal;

    runTest(dInst, expectedVal, expectedReg);
}

void runTest(DecodedInstr dInst, int expectedVal, int expectedReg) {
    int changedReg = -1;
    RegWrite(&dInst, expectedVal, &changedReg);

    int passed = testTrue(expectedReg, changedReg, "RegWrite() wrote to the wrong register");

    if (instructionWritesToRegisters(dInst) == TRUE) {

        int val = mips.registers[expectedReg];
        passed += testTrue(expectedVal, val, "RegWrite() did not write the value into the register");

        printf("PASSED: %d/2 tests \n", passed);

    } else {

        printf("PASSED: %d/1 tests \n", passed);

    }

}