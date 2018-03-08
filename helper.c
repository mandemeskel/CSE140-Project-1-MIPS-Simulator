#include "helper.h"
#include <stdio.h>
#include <assert.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void assertTrue(int expected, int actual, char * msg) {
    if(expected == actual) return;
    printf("%s, expected: %d actual: %d \n", msg, expected, actual);
    assert(FALSE);
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

void printLine(char * msg) {
    printf("%s \n", msg);
}