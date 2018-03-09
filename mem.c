#include <stdio.h>
#include "helper.h"
#include "mem.h"

/*
 * Perform memory load or store. Place the address of any updated memory 
 * in *changedMem, otherwise put -1 in *changedMem. Return any memory value 
 * that is read, otherwise return -1. 
 *
 * Remember that we're mapping MIPS addresses to indices in the mips.memory 
 * array. mips.memory[0] corresponds with address 0x00400000, mips.memory[1] 
 * with address 0x00400004, and so forth.
 *
 */
int Mem( DecodedInstr* d, int val, int *changedMem) {
    logMsg("Mem()");

    if(instructionUsesMem(*d) == FALSE) return -1;

    int address = getAddress(*d);
    
    validateMemoryAddressExists(address);
    validateMemoryAddressWordAligned(address);

    int index = addressIntoMemoryIndex(address);

    if(d->op == SW_OPCODE) {

        mips.memory[index] = d->regs.i.rt;
        *changedMem = address;
        return -1;

    } else if(d->op == LW_OPCODE) {

        return mips.memory[index];

    }

    return -1;
}

/* Check if this instruction writes or reads from memory. */
int instructionUsesMem(DecodedInstr dInstr) {
    return FALSE;
}

/* Returns the memory address associated with the instruction. */
int getAddress(DecodedInstr dInstr) {
    return 0;
}

/* Converts the address into index of SimulatedComputer.memory. */
int addressIntoMemoryIndex(int address) {
    return 0;
}

/* Validates that this address is with in memory. False, print an error message and exit the program. */
void validateMemoryAddressExists(int address) {

}

/* Validates that this address is word aligned. False, print an error message and exit the program. */
void validateMemoryAddressWordAligned(int address) {

}

/* Prints out an error message for invalid address. */
void printAddressErrorMessage(int address) {

}