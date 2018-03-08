#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "computer.h"

void testExecuteAdd();

int main (int argc, char *argv[]) {
    printLine("Running execute test suite...");

    testExecuteAdd();

    printLine("All execute tests passed!");
}

void testExecuteAdd() {
    printf("test add $t8, $t9, $10 - 4 + 5...");
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

    int val = Execute(&dInst, &regVals);
    assertTrue(expectedVal, val, "val does not match expected value");

    printLine("passed");
}

