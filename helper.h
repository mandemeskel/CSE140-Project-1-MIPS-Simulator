#ifndef HELPER
#define HELPER 1

#ifndef COMPUTER
#include "mips.h"
#endif

void assertTrue(int, int, char *);
void logMsg(char *);
void logInstr(char *, unsigned int);
void logDecodedInstr(DecodedInstr*);
void printLine(char *);

#endif