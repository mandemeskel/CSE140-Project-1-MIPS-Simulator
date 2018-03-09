#include "helper.h"
#include <stdio.h>
#include <assert.h>

// turn on logs
const int DEBUGGGING = FALSE;

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
const int SUPPORTED_FUNCTIONS[NUM_SUPPORTED_FUNCTS] = {
  ADDU_FUNCT,
  SUBU_FUNCT,
  SLL_FUNCT,
  SRL_FUNCT,
  AND_FUNCT,
  OR_FUNCT,
  SLT_FUNCT,
  JR_FUNCT
};

// the limits of data addresses in memory
const int START_DATA_ADDRESS = 0x00401000;
const int END_DATA_ADDRESS = 0x404000;
const int WORD_SIZE = 4;

void assertTrue(int expected, int actual, char * msg) {
    if(expected == actual) return;
    printf("%s, expected: %d actual: %d \n", msg, expected, actual);
    assert(FALSE);
}

int testTrue(int expected, int actual, char * msg) {
    if(expected == actual) return TRUE;
    printf("%s, expected: %d actual: %d \n", msg, expected, actual);
    return FALSE;
}

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

void printLine(char * msg) {
    printf("%s \n", msg);
}