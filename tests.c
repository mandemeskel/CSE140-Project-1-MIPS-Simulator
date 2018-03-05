#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "computer.h"

#define TRUE 1
#define FALSE 0

void testDecode();
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
    DecodedInstr actual;
    RegVals rVals;
    DecodedInstr expected = {
        .type = R,
        .op = 0,
        .regs.r.rs = 31,
        .regs.r.rt = 0,
        .regs.r.rd = 0,
        .regs.r.shamt = 0,
        .regs.r.funct = 8                        
    };

    Decode(instr_jr, &actual, &rVals);

    assertTrue(expected.type, actual.type, "wrong instruction type");
    assertTrue(expected.op, actual.op, "wrong opcode");
    assertTrue(expected.regs.r.rs, actual.regs.r.rs, "wrong rs");
    assertTrue(expected.regs.r.rt, actual.regs.r.rt, "wrong rt");
    assertTrue(expected.regs.r.rd, actual.regs.r.rd, "wrong rd");
    assertTrue(expected.regs.r.shamt, actual.regs.r.shamt, "wrong shamt");
    assertTrue(expected.regs.r.funct, actual.regs.r.funct, "wrong funct");

    printLine("Decode tests passed.");
}

void assertTrue(int expected, int actual, char * msg) {
    if(expected == actual) return;
    printf("%s, expected: %d actual: %d \n", msg, expected, actual);
    assert(FALSE);
}

void printLine(char * msg) {
    printf("%s \n", msg);
}