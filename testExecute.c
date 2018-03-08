#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "computer.h"

void testExecuteAdd();
void testExecuteSub();
void runTest(DecodedInstr *, RegVals *, int);

int main (int argc, char *argv[]) {
    printLine("Running execute test suite...");

    testExecuteAdd();
    testExecuteSub();

    printLine("All execute tests passed!");
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
    int param1 = 4, param2 = 5;
    int expectedVal = param1 + param2;

    mips.registers[dInst.regs.r.rs] = param1;
    mips.registers[dInst.regs.r.rt] = param2;
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
    int param1 = 4, param2 = 5;
    int expectedVal = param1 - param2;

    mips.registers[dInst.regs.r.rd] = 0;
    mips.registers[dInst.regs.r.rs] = param1;
    mips.registers[dInst.regs.r.rt] = param2;

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