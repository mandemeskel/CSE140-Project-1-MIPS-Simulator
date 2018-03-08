#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "updatepc.h"

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc+=4;
    /* Your code goes here */
}