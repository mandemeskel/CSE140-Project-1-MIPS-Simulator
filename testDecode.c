#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "computer.h"

#define TRUE 1
#define FALSE 0

void testDecode();
void testDecodeOnJR();
void testDecodeOnBeq();
void testDecodeOnAddiu();
void testDecodeOnJal();
void testDecodeOnXor();
void testDecodeOnSb();
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

    testDecodeOnJR();
    testDecodeOnBeq();
    testDecodeOnAddiu();
    testDecodeOnJal();
    // testDecodeOnXor(); // program exists on successfull test
    // testDecodeOnSb(); // program exists on successfull test

    printLine("Decode tests passed.");
}

void testDecodeOnJR() {
    printf("testing on jr $31...");

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

    printLine("passed");
}

void testDecodeOnBeq() {
    printf("testing on beq $4, $0, 0x00400024...");

    unsigned int instr = 0x10800003;
    RegVals expectedRegVals = {
        .R_rs = 0x1,
        .R_rt = 0,
        .R_rd = 0
    };
    DecodedInstr expectedD = {
        .type = I,
        .op = 4,
        .regs.i.rs = 4,
        .regs.i.rt = 0,
        .regs.i.addr_or_immed = 0x00400024                      
    };

    mips.pc = 0x00400014;
    mips.registers[4] = expectedRegVals.R_rs;
    testInstructionDecode(instr, expectedD, expectedRegVals);

    printLine("passed");
}

void testDecodeOnAddiu() {
    printf("testing on addiu $4, $4, -1...");

    unsigned int instr = 0x2484ffff;
    RegVals expectedRegVals = {
        .R_rs = 0x3,
        .R_rt = 0x3,
        .R_rd = 0
    };
    DecodedInstr expectedD = {
        .type = I,
        .op = 0x9,
        .regs.i.rs = 4,
        .regs.i.rt = 4,
        .regs.i.addr_or_immed = -1                     
    };

    mips.registers[4] = expectedRegVals.R_rs;
    testInstructionDecode(instr, expectedD, expectedRegVals);

    printLine("passed");
}

void testDecodeOnJal() {
    printf("testing on jal 0x00400010...");

    unsigned int instr = 0x0c100004;
    RegVals expectedRegVals = {
        .R_rs = 0,
        .R_rt = 0,
        .R_rd = 0
    };
    DecodedInstr expectedD = {
        .type = J,
        .op = 0x3,
        .regs.j.target = 0x00400010              
    };

    mips.pc = 0x00400008;
    testInstructionDecode(instr, expectedD, expectedRegVals);

    printLine("passed");
}

void testDecodeOnXor() {
    printf("testing on xor t1, t2, t3...");

    unsigned int instr = 0x14B4826;
    RegVals expectedRegVals = {
        .R_rs = 0,
        .R_rt = 0,
        .R_rd = 0
    };
    DecodedInstr expectedD = {
        .type = R,
        .op = 0,
        .regs.r.rs = 9,
        .regs.r.rt = 10,
        .regs.r.rd = 11,
        .regs.r.shamt = 0,
        .regs.r.funct = 38                        
    };

    testInstructionDecode(instr, expectedD, expectedRegVals);

    printLine("passed");
}

void testDecodeOnSb() {
    printf("testing on sb t3 18470(t2)");

    unsigned int instr = 0xA14B4826;
    RegVals expectedRegVals = {
        .R_rs = 0,
        .R_rt = 0,
        .R_rd = 0
    };
    DecodedInstr expectedD = {
        .type = I,
        .op = 0x9,
        .regs.i.rs = 11,
        .regs.i.rt = 10,
        .regs.i.addr_or_immed = 18470                    
    };

    testInstructionDecode(instr, expectedD, expectedRegVals);

    printLine("passed");
}

void testInstructionDecode(unsigned int instruction, DecodedInstr expectedD, RegVals expectedRegVals) {
    DecodedInstr actualD;
    // the default values are not set so rs is equal to 64 by default - idk why
    RegVals rVals = { 
        .R_rs = 0,
        .R_rt = 0,
        .R_rd = 0
    };
    
    Decode(instruction, &actualD, &rVals);

    assertTrue(expectedD.type, actualD.type, "wrong instruction type");
    assertTrue(expectedD.op, actualD.op, "wrong opcode");

    if(expectedD.type == R) {

        assertTrue(expectedD.regs.r.rs, actualD.regs.r.rs, "wrong rs");
        assertTrue(expectedD.regs.r.rt, actualD.regs.r.rt, "wrong rt");
        assertTrue(expectedD.regs.r.rd, actualD.regs.r.rd, "wrong rd");
        assertTrue(expectedD.regs.r.shamt, actualD.regs.r.shamt, "wrong shamt");
        assertTrue(expectedD.regs.r.funct, actualD.regs.r.funct, "wrong funct");

    } else if(expectedD.type == I) {

        assertTrue(expectedD.regs.i.rs, actualD.regs.i.rs, "wrong rs");
        assertTrue(expectedD.regs.i.rt, actualD.regs.i.rt, "wrong rt");
        assertTrue(expectedD.regs.i.addr_or_immed, actualD.regs.i.addr_or_immed, "wrong address or immediate");

    } else {

        assertTrue(expectedD.regs.j.target, actualD.regs.j.target, "wrong target");

    }

    assertTrue(expectedRegVals.R_rs, rVals.R_rs, "wrong rs value");
    assertTrue(expectedRegVals.R_rt, rVals.R_rt, "wrong rt value");
    assertTrue(expectedRegVals.R_rd, rVals.R_rd, "wrong rd value");
}

void assertTrue(int expected, int actual, char * msg) {
    if(expected == actual) return;
    printf("%s, expected: %d actual: %d \n", msg, expected, actual);
    assert(FALSE);
}