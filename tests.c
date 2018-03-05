#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "computer.h"

#define TRUE 1
#define FALSE 0

void testDecode();
void testInstructionDecode(unsigned int, DecodedInstr, RegVals);
void assertTrue(int, int, char *);
void printLine(char *);

int main (int argc, char *argv[]) {
    printLine("Running test suite...");
    
    testDecode();

    printLine("All tests passed!");
}

void testDecode() {
    printLine("Running decode tests.");

    unsigned int instr_jr = 0x03e00008;
    RegVals expectedRegVals = {
        .R_rs = 0x04101023,
        .R_rt = 0,
        .R_rd = 0
    };
    DecodedInstr expectedD = {
        .type = R,
        .op = 0,
        .regs.r.rs = 31,
        .regs.r.rt = 0,
        .regs.r.rd = 0,
        .regs.r.shamt = 0,
        .regs.r.funct = 8                        
    };

    mips.registers[31] = expectedRegVals.R_rs;
    testInstructionDecode(instr_jr, expectedD, expectedRegVals);

    printLine("Decode tests passed.");
}

void testInstructionDecode(unsigned int instruction, DecodedInstr expectedD, RegVals expectedRegVals) {
    DecodedInstr actualD;
    RegVals rVals;

    Decode(instruction, &actualD, &rVals);

    assertTrue(expectedD.type, actualD.type, "wrong instruction type");
    assertTrue(expectedD.op, actualD.op, "wrong opcode");
    assertTrue(expectedD.regs.r.rs, actualD.regs.r.rs, "wrong rs");
    assertTrue(expectedD.regs.r.rt, actualD.regs.r.rt, "wrong rt");
    assertTrue(expectedD.regs.r.rd, actualD.regs.r.rd, "wrong rd");
    assertTrue(expectedD.regs.r.shamt, actualD.regs.r.shamt, "wrong shamt");
    assertTrue(expectedD.regs.r.funct, actualD.regs.r.funct, "wrong funct");

    assertTrue(expectedRegVals.R_rs, rVals.R_rs, "wrong rs value");
    assertTrue(expectedRegVals.R_rt, rVals.R_rt, "wrong rt value");
    assertTrue(expectedRegVals.R_rd, rVals.R_rd, "wrong rd value");
}

void assertTrue(int expected, int actual, char * msg) {
    if(expected == actual) return;
    printf("%s, expected: %d actual: %d \n", msg, expected, actual);
    assert(FALSE);
}

void printLine(char * msg) {
    printf("%s \n", msg);
}