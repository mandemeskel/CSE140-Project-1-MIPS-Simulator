#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <assert.h>
#include "computer.h"

/*
 *  Return an initialized computer with the stack pointer set to the
 *  address of the end of data memory, the remaining registers initialized
 *  to zero, and the instructions read from the given file.
 *  The other arguments govern how the program interacts with the user.
 */
void InitComputer (FILE* filein, int printingRegisters, int printingMemory,
  int debugging, int interactive) {
    int k;
    unsigned int instr;

    /* Initialize registers and memory */

    for (k=0; k<32; k++) {
        mips.registers[k] = 0;
    }
    
    /* stack pointer - Initialize to highest address of data segment */
    mips.registers[29] = 0x00400000 + (MAXNUMINSTRS+MAXNUMDATA)*4;

    for (k=0; k<MAXNUMINSTRS+MAXNUMDATA; k++) {
        mips.memory[k] = 0;
    }

    k = 0;
    while (fread(&instr, 4, 1, filein)) {
	/*swap to big endian, convert to host byte order. Ignore this.*/
        mips.memory[k] = ntohl(endianSwap(instr));
        k++;
        if (k>MAXNUMINSTRS) {
            fprintf (stderr, "Program too big.\n");
            exit (1);
        }
    }

    mips.printingRegisters = printingRegisters;
    mips.printingMemory = printingMemory;
    mips.interactive = interactive;
    mips.debugging = debugging;
}

unsigned int endianSwap(unsigned int i) {
    return (i>>24)|(i>>8&0x0000ff00)|(i<<8&0x00ff0000)|(i<<24);
}

/*
 *  Run the simulation.
 */
void Simulate () {
    char s[40];  /* used for handling interactive input */
    unsigned int instr;
    int changedReg=-1, changedMem=-1, val, loadedVal;
    DecodedInstr d;
    
    /* Initialize the PC to the start of the code section */
    mips.pc = 0x00400000;
    while (1) {
        if (mips.interactive) {
            printf ("> ");
            fgets (s,sizeof(s),stdin);
            if (s[0] == 'q') {
                return;
            }
        }

        /* Fetch instr at mips.pc, returning it in instr */
        instr = Fetch (mips.pc);

        printf ("Executing instruction at %8.8x: %8.8x\n", mips.pc, instr);

        /* 
	 * Decode instr, putting decoded instr in d
	 * Note that we reuse the d struct for each instruction.
	 */
        Decode (instr, &d, &rVals);

        /*Print decoded instruction*/
        PrintInstruction(&d);

        /* 
	 * Perform computation needed to execute d, returning computed value 
	 * in val 
	 */
        val = Execute(&d, &rVals);

	UpdatePC(&d,val);

        /* 
	 * Perform memory load or store. Place the
	 * address of any updated memory in *changedMem, 
	 * otherwise put -1 in *changedMem. 
	 * Return any memory value that is read, otherwise return -1.
         */
        loadedVal = Mem(&d, val, &changedMem);

        if(d.op == LW_OPCODE) val = loadedVal;

        /* 
	 * Write back to register. If the instruction modified a register--
	 * (including jal, which modifies $ra) --
         * put the index of the modified register in *changedReg,
         * otherwise put -1 in *changedReg.
         */
        RegWrite(&d, val, &changedReg);

        PrintInfo (changedReg, changedMem);
    }
}

/*
 *  Print relevant information about the state of the computer.
 *  changedReg is the index of the register changed by the instruction
 *  being simulated, otherwise -1.
 *  changedMem is the address of the memory location changed by the
 *  simulated instruction, otherwise -1.
 *  Previously initialized flags indicate whether to print all the
 *  registers or just the one that changed, and whether to print
 *  all the nonzero memory or just the memory location that changed.
 */
void PrintInfo ( int changedReg, int changedMem) {
    int k, addr;
    printf ("New pc = %8.8x\n", mips.pc);
    if (!mips.printingRegisters && changedReg == -1) {
        printf ("No register was updated.\n");
    } else if (!mips.printingRegisters) {
        printf ("Updated r%2.2d to %8.8x\n",
        changedReg, mips.registers[changedReg]);
    } else {
        for (k=0; k<32; k++) {
            printf ("r%2.2d: %8.8x  ", k, mips.registers[k]);
            if ((k+1)%4 == 0) {
                printf ("\n");
            }
        }
    }
    if (!mips.printingMemory && changedMem == -1) {
        printf ("No memory location was updated.\n");
    } else if (!mips.printingMemory) {
        printf ("Updated memory at address %8.8x to %8.8x\n",
        changedMem, Fetch (changedMem));
    } else {
        printf ("Nonzero memory\n");
        printf ("ADDR	  CONTENTS\n");
        for (addr = 0x00400000+4*MAXNUMINSTRS;
             addr < 0x00400000+4*(MAXNUMINSTRS+MAXNUMDATA);
             addr = addr+4) {
            if (Fetch (addr) != 0) {
                printf ("%8.8x  %8.8x\n", addr, Fetch (addr));
            }
        }
    }
}

/*
 *  Return the contents of memory at the given address. Simulates
 *  instruction fetch. 
 */
unsigned int Fetch ( int addr) {
    return mips.memory[(addr-0x00400000)/4];
}

/*
 *  Print the disassembled version of the given instruction
 *  followed by a newline.
 */
void PrintInstruction ( DecodedInstr* d) {
    char * instName = getInstructionName(*d);
    int arg1, arg2, arg3;

    if(d->op == R_FORMAT_OPCODE) {

        arg1 = d->regs.r.rd;
        arg2 = d->regs.r.rt;

        if(d->regs.r.funct == SLL_FUNCT || d->regs.r.funct == SRL_FUNCT) {

            arg3 = d->regs.r.shamt;
            printf("%s\t$%d, $%d, %d\n", instName, arg1, arg2, arg3);

        } else if(d->regs.r.funct == JR_FUNCT) {

            arg1 = d->regs.r.rs;
            printf("%s\t$%d\n", instName, arg1);

        } else {

            arg3 = d->regs.r.rs;
            printf("%s\t$%d, $%d, $%d\n", instName, arg1, arg2, arg3);

        }

    } else if(d->type == J) {

        arg1 = d->regs.j.target;
        printf("%s\t0x00%x\n", instName, arg1);

    } else {

        arg1 = d->regs.i.rs;
        arg2 = d->regs.i.rt;
        arg3 = d->regs.i.addr_or_immed;

        if(d->op == BNE_OPCODE || d->op == BEQ_OPCODE) {

            printf("%s\t$%d, $%d, 0x00%x\n", instName, arg1, arg2, arg3);

        } else if(d->op == ORI_OPCODE || d->op == ANDI_OPCODE) {

            printf("%s\t$%d, $%d, 0x00%x\n", instName, arg1, arg2, arg3) ;

        } else if(d->op == SW_OPCODE || d->op == LW_OPCODE) {

            printf("%s\t$%d, %d($%d)\n", instName, arg1, arg3, arg2);

        } else if(d->op == LUI_OPCODE) {

            printf("%s\t$%d, 0x00%x\n", instName, arg2, arg3);

        } else if(d->op == ADDIU_OPCODE) {

            printf("%s\t$%d, $%d, %d\n", instName, arg1, arg2, arg3);
            
        }

    }
    
}

/* Returns the name of the instruciton. */
char * getInstructionName ( DecodedInstr d) {
    if(d.op == ADDIU_OPCODE)
        return "addiu";
    else if(d.op == ANDI_OPCODE)
        return "andi";
    else if(d.op == ORI_OPCODE)
        return "ori";
    else if(d.op == LUI_OPCODE)
        return "lui";
    else if(d.op == BEQ_OPCODE)
        return "beq";
    else if(d.op == BNE_OPCODE)
        return "bne";
    else if(d.op == J_OPCODE)
        return "j";
    else if(d.op == JAL_OPCODE)
        return "jal";
    else if(d.op == LW_OPCODE)
        return "lw";
    else if(d.op == SW_OPCODE)
        return "sw";
    else if(d.op == R_FORMAT_OPCODE) {

        if(d.regs.r.funct == ADDU_FUNCT)
            return "addu";
        else if(d.regs.r.funct == SUBU_FUNCT)
            return "subu";
        else if(d.regs.r.funct == SLL_FUNCT)
            return "sll";
        else if(d.regs.r.funct == SRL_FUNCT)
            return "srl";
        else if(d.regs.r.funct == AND_FUNCT)
            return "and";
        else if(d.regs.r.funct == OR_FUNCT)
            return "or";
        else if(d.regs.r.funct == SLT_FUNCT)
            return "slt";
        else if(d.regs.r.funct == JR_FUNCT)
            return "jr";
        
    }

    return "inop";
}

/* 
 * Write back to register. If the instruction modified a register--
 * (including jal, which modifies $ra) --
 * put the index of the modified register in *changedReg,
 * otherwise put -1 in *changedReg.
 */
void RegWrite( DecodedInstr* d, int val, int *changedReg) {
    logMsg("RegWrite()");

    if(instructionWritesToRegisters(*d) == FALSE) return;

    if(d->op == R_FORMAT_OPCODE) {

        *changedReg = d->regs.r.rd;

    } else if(d->type == I) {

        *changedReg = d->regs.i.rt;

    } else {

        return;

    }

    mips.registers[*changedReg] = val;

}

/* Checks if the instruction writes to a register. */
int instructionWritesToRegisters(DecodedInstr d) {
    int isRFormat = d.op == R_FORMAT_OPCODE,
        isLw = d.op == LW_OPCODE,
        isAddiu = d.op == ADDIU_OPCODE,
        isAndi = d.op == ANDI_OPCODE,
        isOri = d.op == ORI_OPCODE,
        isLui = d.op == LUI_OPCODE;
    int writesToRegister = isRFormat + isLw + isAddiu + isAndi + isOri + isLui;

    return writesToRegister;
}