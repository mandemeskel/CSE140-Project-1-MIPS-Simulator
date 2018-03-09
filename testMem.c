#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "mem.h"

void testExecuteAddu();
void testExecuteSw();
void testExecuteLw();
void testExecuteInvalid();
void testExecuteMissAligned();
void runTest(DecodedInstr, int, int, int);

int main (int argc, char *argv[]) {
    printLine("Running Mem() test suite...");

    testExecuteAddu();
    testExecuteSw();
    testExecuteLw();
    testExecuteInvalid();
    testExecuteMissAligned();

    printLine("Finished running tests.");
}

void testExecuteAddu() {
    printLine("test addu $11, $12, $13...");
    DecodedInstr dInst = {
        .type = R,
        .op = R_FORMAT_OPCODE,
        .regs.r.rs = 12,
        .regs.r.rt = 13,
        .regs.r.rd = 11,
        .regs.r.shamt = 0,
        .regs.r.funct = ADDU_FUNCT
    };
    int expectedWriteValue = 0, expectedReadValue = -1, expectedChangedMem = 0;

    runTest(dInst, expectedWriteValue, expectedReadValue, expectedChangedMem);
}

void testExecuteSw() {
    printLine("test sw $11, 4($12)...");
    DecodedInstr dInst = {
        .type = I,
        .op = SW_OPCODE,
        .regs.i.rt = 11,
        .regs.i.rs = 12,
        .regs.i.addr_or_immed = 4
    };
    int expectedWriteValue = 10, expectedReadValue = -1, expectedChangedMem = START_DATA_ADDRESS + dInst.regs.i.addr_or_immed;

    mips.registers[dInst.regs.i.rs] = START_DATA_ADDRESS;
    mips.registers[dInst.regs.i.rt] = expectedWriteValue;

    runTest(dInst, expectedWriteValue, expectedReadValue, expectedChangedMem);
}

void testExecuteLw() {
    printLine("test lw $11, -4($12)...");
    DecodedInstr dInst = {
        .type = I,
        .op = SW_OPCODE,
        .regs.i.rt = 11,
        .regs.i.rs = 12,
        .regs.i.addr_or_immed = -4
    };
    int expectedWriteValue = 0, expectedReadValue = 12, expectedChangedMem = -1, readAddress = END_DATA_ADDRESS + dInst.regs.i.addr_or_immed;
    int memIndex = (readAddress - START_DATA_ADDRESS)/4;

    mips.memory[memIndex] = expectedReadValue;

    runTest(dInst, expectedWriteValue, expectedReadValue, expectedChangedMem);
}

void testExecuteInvalid() {
    printLine("test invalid address: lw $11, 4($12)...");
    DecodedInstr dInst = {
        .type = I,
        .op = SW_OPCODE,
        .regs.i.rt = 11,
        .regs.i.rs = 12,
        .regs.i.addr_or_immed = 4
    };
    int expectedWriteValue = 0, expectedReadValue = 12, expectedChangedMem = -1, readAddress = END_DATA_ADDRESS + dInst.regs.i.addr_or_immed;
    int memIndex = (readAddress - START_DATA_ADDRESS)/4;

    mips.memory[memIndex] = expectedReadValue;

    runTest(dInst, expectedWriteValue, expectedReadValue, expectedChangedMem);
}

void testExecuteMissAligned() {
    printLine("test missaligned address: sw $11, 3($12)...");
    DecodedInstr dInst = {
        .type = I,
        .op = SW_OPCODE,
        .regs.i.rt = 11,
        .regs.i.rs = 12,
        .regs.i.addr_or_immed = 3
    };
    int expectedWriteValue = 12, expectedReadValue = -1, expectedChangedMem = -1;

    mips.registers[dInst.regs.i.rs] = START_DATA_ADDRESS;
    mips.registers[dInst.regs.i.rt] = expectedWriteValue;

    runTest(dInst, expectedWriteValue, expectedReadValue, expectedChangedMem);
}

void runTest(DecodedInstr dInst, int expectedWriteValue, int expectedReadValue, int expectedChangedMem) {
    int changedMem = -1;
    int memValue = Mem(&dInst, expectedWriteValue, &changedMem);

    int passed = testTrue(expectedChangedMem, changedMem, "\t Mem() changed the wrong memory address");

    passed += testTrue(expectedReadValue, memValue, "\t Mem() returned the wrong value");

    int address = getAddress(dInst);
    int index = addressIntoMemoryIndex(address);
    int actualValue = mips.memory[index];
    passed += testTrue(expectedWriteValue, actualValue, "\t Mem() wrote the wrong value into memory");

    printf("PASSED: %d/3 tests \n", passed);
}