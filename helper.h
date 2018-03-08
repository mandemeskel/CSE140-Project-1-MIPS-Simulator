#ifndef HELPER
#define HELPER 1

#ifndef COMPUTER
#include "mips.h"
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void assertTrue(int, int, char *);
int testTrue(int, int, char *);
void logMsg(char *);
void logInstr(char *, unsigned int);
void logDecodedInstr(DecodedInstr*);
void printLine(char *);

#ifndef R_FORMAT_OPCODE
// supported instructions and there opcode
extern const int R_FORMAT_OPCODE;
extern const int ADDIU_OPCODE;
extern const int ANDI_OPCODE;
extern const int ORI_OPCODE;
extern const int LUI_OPCODE;
extern const int BEQ_OPCODE;
extern const int BNE_OPCODE;
extern const int J_OPCODE;
extern const int JAL_OPCODE;
extern const int LW_OPCODE;
extern const int SW_OPCODE;
extern const int NUM_SUPPORTED_CODES;
extern const int SUPPORTED_OPCODES[11];
// supported instructions and there function codes
extern const int ADDU_FUNCT;
extern const int SUBU_FUNCT;
extern const int SLL_FUNCT;
extern const int SRL_FUNCT;
extern const int AND_FUNCT;
extern const int OR_FUNCT;
extern const int SLT_FUNCT;
extern const int JR_FUNCT;
extern const int NUM_SUPPORTED_FUNCTS;
extern const int SUPPORTED_FUNCTIONS[8];
#endif

#endif