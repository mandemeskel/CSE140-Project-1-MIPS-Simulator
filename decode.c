#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "decode.h"

// supported instructions and there opcode
const int R_FORMAT_OPCODE = 0,
  ADDIU_OPCODE = 9,
  ANDI_OPCODE = 12,
  ORI_OPCODE = 13,
  LUI_OPCODE = 15,
  BEQ_OPCODE = 4,
  BNE_OPCODE = 5,
  J_OPCODE = 2,
  JAL_OPCODE = 3,
  LW_OPCODE = 35,
  SW_OPCODE = 43;
const int NUM_SUPPORTED_CODES = 11;
const int SUPPORTED_OPCODES[NUM_SUPPORTED_CODES] = {
  R_FORMAT_OPCODE,
  ADDIU_OPCODE,
  ANDI_OPCODE,
  ORI_OPCODE,
  LUI_OPCODE,
  BEQ_OPCODE,
  BNE_OPCODE,
  J_OPCODE,
  JAL_OPCODE,
  LW_OPCODE,
  SW_OPCODE
};

// supported instructions and there function codes
const int ADDU_FUNCT = 33,
  SUBU_FUNCT = 35,
  SLL_FUNCT = 0,
  SRL_FUNCT = 2,
  AND_FUNCT = 36,
  OR_FUNCT = 37,
  SLT_FUNCT = 42,
  JR_FUNCT = 8;
const int NUM_SUPPORTED_FUNCTS = 8;
const int SUPPORTED_FUNCTIONS[NUM_SUPPORTED_CODES] = {
  ADDU_FUNCT,
  SUBU_FUNCT,
  SLL_FUNCT,
  SRL_FUNCT,
  AND_FUNCT,
  OR_FUNCT,
  SLT_FUNCT,
  JR_FUNCT
};

/* Decode instr, returning decoded instruction. */
void Decode ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    logInstr("Decode()", instr);

    d->op = findOpcode(instr);
    
    validateInstructionOpcode(d->op);

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

/* Ensure that this instruction is supported based on its opcode, if not exist. */
void validateInstructionOpcode ( int opcode) {
    logInstr("validateInstructionOpcode()", opcode);

    for(int index = 0; index <= NUM_SUPPORTED_CODES; index++) {

        if(opcode == SUPPORTED_OPCODES[index]) return;

    }

    exit(0);
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

    validateInstructionFunction(funct);

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

/* Ensure that this instruction is supported based on its ALU function, if not exist. */
void validateInstructionFunction ( int funct) {
    logInstr("validateInstructionFunction()", funct);

    for(int index = 0; index <= NUM_SUPPORTED_FUNCTS; index++) {

        if(funct == SUPPORTED_FUNCTIONS[index]) return;

    }

    exit(0);
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

    if(isBranch(d->op))
        imm = signExtendBranchAddress(imm);
    else
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

/* Sign extends an immediate for use by branch instruction. */
int signExtendBranchAddress ( int imm) {
    logInstr("signExtendBranchAddress()", imm);
    assert(imm <= 65535); // highest number a 16 bit immediate could be

    imm = imm << 2;
    imm += 4;
    imm += mips.pc;

    return imm;
}

/* Returns true if this is a branch instruction beq or bne. */
int isBranch ( int opcode) {
    return opcode == 4 || opcode == 5;
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

const int DEBUGGGING = 1;
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