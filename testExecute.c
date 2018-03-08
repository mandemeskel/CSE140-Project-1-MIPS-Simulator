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
void runTest(DecodedInstr *, RegVals *, int);

int main (int argc, char *argv[]) {
    printLine("Running execute test suite...");

    testExecuteAdd();
    testExecuteSub();
    testExecuteSll();
    testExecuteSrl();
    testExecuteAnd();
    testExecuteOr();
    testExecuteLui();

    printLine("Finished running tests.");
}

void testExecuteAdd() {
    printf("test addu $11, $12, $13: 4 + 5...");
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.rd = 11,
        .regs.r.shamt = 0,
        .regs.r.funct = ADDU_FUNCT
    };
    RegVals regVals;
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue + rtValue;

    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;
    mips.registers[dInst.regs.r.rd] = 0;

    runTest(&dInst, &regVals, expectedVal);
}

void testExecuteSub() {
    printf("test subu $11, $12, $13: 4 - 5...");
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rd = 11,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.shamt = 0,
        .regs.r.funct = SUBU_FUNCT
    };
    RegVals regVals;
    int rsValue = 4, rtValue = 5;
    int expectedVal = rsValue - rtValue;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(&dInst, &regVals, expectedVal);
}

void testExecuteSll() {
    printf("test sll $11, $13, 4: 5 << 4...");
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
    RegVals regVals;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = 0;
    mips.registers[dInst.regs.r.rt] = rtValues;

    runTest(&dInst, &regVals, expectedVal);
}

void testExecuteSrl() {
    printf("test srl $11, $13, 4: 80 >> 4...");
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
    RegVals regVals;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = 0;
    mips.registers[dInst.regs.r.rt] = rtValues;

    runTest(&dInst, &regVals, expectedVal);
}

void testExecuteAnd() {
    printf("test and $11, $12, $13: 4 & 5...");
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
    RegVals regVals;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(&dInst, &regVals, expectedVal);
}

void testExecuteOr() {
    printf("test or $11, $12, $13: 4 | 5...");
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
    RegVals regVals;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = rsValue;
    mips.registers[dInst.regs.r.rt] = rtValue;

    runTest(&dInst, &regVals, expectedVal);
}

void testExecuteLui() {
    printf("test lui $11, 4: 4 << 16...");
    int imm = 4;
    int expectedVal = imm << 16;
    DecodedInstr dInst = {
        .type = I,
        .op = LUI_OPCODE,
        .regs.i.rt = 11,
        .regs.i.addr_or_immed = imm
    };
    RegVals regVals;

    runTest(&dInst, &regVals, expectedVal);
}

void runTest(DecodedInstr * dInst, RegVals * regVals, int expectedVal) {
    int val = Execute(dInst, regVals);
    int passed = testTrue(expectedVal, val, "Execute() returned wrong value");

    if(passed == TRUE)
        printLine("PASSEDS");
    else
        printLine("FAILED");
}