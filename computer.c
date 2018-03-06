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
    int changedReg=-1, changedMem=-1, val;
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
        val = Mem(&d, val, &changedMem);

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

/* Decode instr, returning decoded instruction. */
void Decode ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    logInstr("Decode()", instr);

    d->op = findOpcode(instr);
    d->type = findInstructionType(d->op);

    if (d->type == R) 
        decodeRFormat(instr, d, rVals);
    else if (d->type == I) 
        decodeIFormat(instr, d, rVals);
    else 
        decodeJFormat(instr, d, rVals);
    
    logDecodedInstr(d);
}

const unsigned int OPCODE_AND_OP = 0b11111100000000000000000000000000;
const unsigned short OPCODE_BIT_START_LOCATION = 26;
/* Returns the opcode of the instruction. */
unsigned short findOpcode ( unsigned int instr) {
    logInstr("findOpcode()", instr);

    unsigned int opcode = instr & OPCODE_AND_OP;
    opcode = opcode >> OPCODE_BIT_START_LOCATION;
    return opcode;
}

/* Returns the instruction type base on the opcode. */
InstrType findInstructionType ( unsigned short opcode) {
    logInstr("findInstructionType()", opcode);
    // the highest opcode we should get is 43 for sw
    assert(opcode <= 43);

    if (opcode == 0) return R;
    if (opcode == 2 || opcode == 3) return J;
    return I;
}

const unsigned int RS_AND_OP = 0b00000011111000000000000000000000;
const unsigned int RT_AND_OP = 0b00000000000111110000000000000000;
const unsigned int RD_AND_OP = 0b00000000000000001111100000000000;
const unsigned int SH_AND_OP = 0b00000000000000000000011111000000;
const unsigned int FN_AND_OP = 0b00000000000000000000000000111111;
const short RS_BIT_START_LOCATION = 21;
const short RT_BIT_START_LOCATION = 16;
const short RD_BIT_START_LOCATION = 11;
const short SH_BIT_START_LOCATION = 6;
/* Decodes R format intructions. */
void decodeRFormat ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    logInstr("decodeRFormat()", instr);
    assert(d->type == R);

    unsigned int rs = instr & RS_AND_OP;
    rs = rs >> RS_BIT_START_LOCATION;

    unsigned int rt = instr & RT_AND_OP;
    rt = rt >> RT_BIT_START_LOCATION;
    
    unsigned int rd = instr & RD_AND_OP;
    rd = rd >> RD_BIT_START_LOCATION;

    unsigned int shamt = instr & SH_AND_OP;
    shamt = shamt >> SH_BIT_START_LOCATION;

    unsigned int funct = instr & FN_AND_OP;

    d->regs.r.rs = rs;
    d->regs.r.rt = rt;
    d->regs.r.rd = rd;
    d->regs.r.shamt = shamt;
    d->regs.r.funct = funct;

    rVals->R_rs = mips.registers[rs];
    rVals->R_rt = mips.registers[rt];
    rVals->R_rd = mips.registers[rd];

    logDecodedInstr(d);
}

const unsigned int IMM_AND_OP = 0b00000000000000001111111111111111;
/* Decodes I format intructions. */
void decodeIFormat ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    logInstr("decodeIFormat()", instr);
    assert(d->type == I);

    unsigned int rs = instr & RS_AND_OP;
    rs = rs >> RS_BIT_START_LOCATION;

    unsigned int rt = instr & RT_AND_OP;
    rt = rt >> RT_BIT_START_LOCATION;
    
    int imm = instr & IMM_AND_OP;
    imm = signExtendImmediate(imm);

    d->regs.i.rs = rs;
    d->regs.i.rt = rt;
    d->regs.i.addr_or_immed = imm;

    rVals->R_rs = mips.registers[rs];
    rVals->R_rt = mips.registers[rt];

    logDecodedInstr(d);
}

const unsigned int IMM_LAST_BIT_LOCATION = 15;
const unsigned int IMM_SIGN_EXTENDER = 0b11111111111111110000000000000000;
/* Sign extend an immediate for an I format instruction. */
int signExtendImmediate(int imm) {
    logInstr("signExtendImmediate()", imm);
    assert(imm <= 65535); // highest number a 16 bit immediate could be

    unsigned short lastBit = imm >> IMM_LAST_BIT_LOCATION;

    if(lastBit == 0) return imm;

    return imm | IMM_SIGN_EXTENDER;
}

const unsigned int TARGET_AND_OP = 0b00000011111111111111111111111111;
/* Decodes J format intructions. */
void decodeJFormat ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    assert(d->type == J);

    int target = instr & TARGET_AND_OP;
    target = target << 2;

    int four_most_bits = mips.pc >> 30;
    four_most_bits = four_most_bits << 30;
    target = target | four_most_bits;

    d->regs.j.target = target;

    logDecodedInstr(d);
}

const unsigned int ADDRESS_LAST_BIT_LOCATION = 25;
const unsigned int ADDRESS_SIGN_EXTENDER = 0b11111110000000000000000000000000;
/* Sign extend an address for an J format instruction. */
int signExtendAddress ( int address) {
    logInstr("signExtendAddress()", address);
    assert(address <= 33554431); // highest number a 16 bit address could be

    unsigned short lastBit = address >> ADDRESS_LAST_BIT_LOCATION;

    if(lastBit == 0) return address;

    return address | ADDRESS_SIGN_EXTENDER;
}

/*
 *  Print the disassembled version of the given instruction
 *  followed by a newline.
 */
void PrintInstruction ( DecodedInstr* d) {
    /* Your code goes here */
}

/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
  return 0;
}

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc+=4;
    /* Your code goes here */
}

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
    /* Your code goes here */
  return 0;
}

/* 
 * Write back to register. If the instruction modified a register--
 * (including jal, which modifies $ra) --
 * put the index of the modified register in *changedReg,
 * otherwise put -1 in *changedReg.
 */
void RegWrite( DecodedInstr* d, int val, int *changedReg) {
    /* Your code goes here */
}

const int DEBUGGGING = 0;
/* Print out debug messages to terminal. */
void logMsg ( char * msg) {
    if(DEBUGGGING == 0) return;
    printf("%s \n", msg);
}

/* Print out debug messages with instruction. */
void logInstr (char * msg, unsigned int instr) {
    if(DEBUGGGING == 0) return;
    printf("%s: %8.8x \n", msg, instr);
}

/* Print out the contents of DecodedInstr. */
void logDecodedInstr(DecodedInstr* d) {
    if(DEBUGGGING == 0) return;
    printf("instruction type: %d \n", d->type);
    printf("op code: %d \n", d->op);
    
    if(d->type == R) {

        printf("R format \n");
        printf("rs: %d \n", d->regs.r.rs);
        printf("rt: %d \n", d->regs.r.rt);
        printf("rd: %d \n", d->regs.r.rd);
        printf("shamt: %d \n", d->regs.r.shamt);
        printf("funct: %d \n", d->regs.r.funct);

    } else if(d->type == I) {

        printf("I format \n");
        printf("rs: %d \n", d->regs.i.rs);
        printf("rt: %d \n", d->regs.i.rt);
        printf("address or immediate: %d \n", d->regs.i.addr_or_immed);

    } else {

        printf("J format \n");
        printf("target: %8.8x \n", d->regs.j.target);

    }
}