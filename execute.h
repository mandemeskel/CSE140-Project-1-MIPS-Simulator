#ifndef EXECUTE
#define EXECUTE 1

#ifndef COMPUTER
#include "mips.h"
#endif

int Execute (DecodedInstr*, RegVals*);
int getAluOperation (DecodedInstr*);
int functionCodeToAluOp (int);

int getParam1 (int (int, int), DecodedInstr*);
int getRFormatParam1 (int (int, int), DecodedInstr*);
int getIFormatParam1 (int (int, int), DecodedInstr*);

int getParam2 (int (int, int), DecodedInstr*);
int getRFormatParam2 (int (int, int), DecodedInstr*);
int getIFormatParam2 (int (int, int), DecodedInstr*);

int executeOperation (int (int, int), int, int);

int addOp (int, int);
int subOp (int, int);
int sllOp (int, int);
int srlOp (int, int);
int andOp (int, int);
int orOp (int, int);
int sltOp (int, int);
// int luiOp (int, int);

#endif