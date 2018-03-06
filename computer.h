
#define MAXNUMINSTRS 1024	/* max # instrs in a program */
#define MAXNUMDATA 3072		/* max # data words */
#undef mips			/* gcc already has a def for mips */

struct SimulatedComputer {
    int memory [MAXNUMINSTRS+MAXNUMDATA];
    int registers [32];
    int pc;
    int printingRegisters, printingMemory, interactive, debugging;
};
typedef struct SimulatedComputer Computer;

typedef enum { R=0, I, J } InstrType;

typedef struct {
  int rs;
  int rt;
  int rd;
  int shamt;
  int funct;
} RRegs;

typedef struct {
  int rs;
  int rt;
  int addr_or_immed;
} IRegs;

typedef struct {
  int target;
} JRegs;

typedef struct {
  InstrType type;
  int op;
  union {
    RRegs r;
    IRegs i;
    JRegs j;
  } regs;
} DecodedInstr;

typedef struct {
  int R_rs; /*Value in register rs*/
  int R_rt;
  int R_rd;
} RegVals;

void InitComputer (FILE*, int printingRegisters, int printingMemory,
    int debugging, int interactive);
void Simulate ();

unsigned int endianSwap(unsigned int);

void PrintInfo (int changedReg, int changedMem);
unsigned int Fetch (int);

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

int Execute (DecodedInstr*, RegVals*);
int Mem(DecodedInstr*, int, int *);
void RegWrite(DecodedInstr*, int, int *);
void UpdatePC(DecodedInstr*, int);
void PrintInstruction (DecodedInstr*);

void logMsg(char *);
void logInstr(char *, unsigned int);
void logDecodedInstr(DecodedInstr*);

/*Globally accessible Computer variable*/
Computer mips;
RegVals rVals;
