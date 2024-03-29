#ifndef MY_MIPS
#include "mips.h"
#endif

#ifndef HELPER
#include "helper.h"
#endif

#ifndef DECODE
#include "decode.h"
#endif

#ifndef EXECUTE
#include "execute.h"
#endif

#ifndef UPDATEPC
#include "updatepc.h"
#endif

#ifndef MEM
#include "mem.h"
#endif

void InitComputer (FILE*, int printingRegisters, int printingMemory,
    int debugging, int interactive);
void Simulate ();

unsigned int endianSwap(unsigned int);

void PrintInfo (int changedReg, int changedMem);
unsigned int Fetch (int);

void RegWrite(DecodedInstr*, int, int *);
int instructionWritesToRegisters(DecodedInstr);

void PrintInstruction (DecodedInstr*);
char * getInstructionName (DecodedInstr);

/*Globally accessible Computer variable*/
Computer mips;
RegVals rVals;
