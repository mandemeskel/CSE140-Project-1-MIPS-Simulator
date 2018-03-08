#ifndef DECODE
#define DECODE 1

#ifndef COMPUTER
#include "mips.h"
#endif

void Decode (unsigned int, DecodedInstr*, RegVals*);
unsigned short findOpcode(unsigned int);
InstrType findInstructionType(unsigned short);
void validateInstructionOpcode(int);
void validateInstructionFunction(int);
void decodeRFormat(unsigned int, DecodedInstr*, RegVals*);
void decodeIFormat(unsigned int, DecodedInstr*, RegVals*);
int signExtendImmediate(int);
int signExtendBranchAddress(int);
int isBranch(int);
void decodeJFormat(unsigned int, DecodedInstr*, RegVals*);
int signExtendAddress(int address);

#endif