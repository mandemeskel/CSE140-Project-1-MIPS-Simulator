#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "updatepc.h"

// register ra's index in mips.registers
int const R_RA = 31;

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc+=4;
    
    if (d->op == BEQ_OPCODE) {

        if(val == 0) mips.pc = d->regs.i.addr_or_immed;
        return;

    } else if (d->op == BNE_OPCODE) {

        if(val != 0) mips.pc = d->regs.i.addr_or_immed;
        return;

    } else if (d->op == JAL_OPCODE) {

        mips.registers[R_RA] = mips.pc; 
        mips.pc = d->regs.j.target;
        return;

    } else if (d->op == J_OPCODE) {

        mips.pc = d->regs.j.target;
        return;

    } else if (d->op == R_FORMAT_OPCODE && d->regs.r.funct == JR_FUNCT) {

        mips.pc = mips.registers[d->regs.r.rs];
        return;

    }
}