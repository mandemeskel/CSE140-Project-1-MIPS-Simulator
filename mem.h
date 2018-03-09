#ifndef MEM
#define MEM 1

#ifndef COMPUTER
#include "mips.h"
#endif

int Mem(DecodedInstr*, int, int *);
int getAddress(DecodedInstr);
int addressIntoMemoryIndex(int);
void validateMemoryAddressExists(int);
void validateMemoryAddressWordAligned(int);
void printAddressErrorMessage(int);

#endif