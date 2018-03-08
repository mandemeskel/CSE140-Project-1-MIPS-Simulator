#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "decode.h"
#include "execute.h"

enum AluOperation {
    ADD_OP = 1,
    SUB_OP,
    OR_OP,
    AND_OP,
    SLL_OP,
    SRL_OP,
    SLT_OP
};

/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) {
    logMsg("Execute()");
    
    int aluOp = getAluOperation(d);
    int param1 = getParam1(aluOp, d);
    int param2 = getParam2(aluOp, d);
    int val = executeOperation(aluOp, param1, param2);

    return val;
}

/* Returns the ALU operation of this instructions. */
int getAluOperation (DecodedInstr* d) {
    logMsg("getAluOperation()");

    int opcode = d->op;

    if(opcode == R_FORMAT_OPCODE) {
        
        return functionCodeToAluOp(d->regs.r.funct);

    } else if(
        opcode == ADDIU_OPCODE || 
        opcode == LW_OPCODE || opcode == SW_OPCODE) {

        return ADD_OP;

    } else if(opcode == BNE_OPCODE || opcode == BEQ_OPCODE) {

        return SUB_OP;

    } else if(opcode == LUI_OPCODE) {

        return SLL_OP;

    } else if(opcode == ORI_OPCODE) {

        return OR_OP;

    } else if(opcode == ANDI_OPCODE) {

        return AND_OP;

    }
    
    return -1;
}

/* Returns the ALU operation for this function code. */
int functionCodeToAluOp (int functionCode) {
    logMsg("functionCodeToAluOp()");

    if(functionCode == ADDU_FUNCT) {

        return ADD_OP;

    } else if(functionCode == SUBU_FUNCT) {

        return SUB_OP;

    } else if(functionCode == AND_FUNCT) {

        return AND_OP;

    } else if(functionCode == OR_FUNCT) {

        return OR_OP;

    } else if(functionCode == SLL_FUNCT) {

        return SLL_OP;

    } else if(functionCode == SRL_FUNCT) {

        return SRL_OP;

    } else if(functionCode == SLT_FUNCT) {

        return SLT_OP;

    }

    return -1;
}

/* Returns the first parameter of the ALU operations. */
int getParam1 (int aluOperation, DecodedInstr* d) {
    return 0;
}

/* Returns the first parameter of the ALU operations for R format instructions. */
int getRFormatParam1 (int aluOperation, DecodedInstr* d) {
    return 0;
}

/* Returns the first parameter of the ALU operations for I format instructions. */
int getIFormatParam1 (int aluOperation, DecodedInstr* d) {
    return 0;
}

/* Returns the second parameter of the ALU operations. */
int getParam2 (int aluOperation, DecodedInstr* d) {
    return 0;
}

/* Returns the second parameter of the ALU operations for R format instructions. */
int getRFormatParam2 (int aluOperation, DecodedInstr* d) {
    return 0;
}

/* Returns the second parameter of the ALU operations for I format instructions. */
int getIFormatParam2 (int aluOperation, DecodedInstr* d) {
    return 0;
}

/* Executes the ALU operation and returns the value. */
int executeOperation (int aluOperation, int param1, int param2) {
    return 0;
}

/* ALU operation for Execute, param1 + param2. */
int addOp (int param1, int param2) {
    return param1 + param2;
}

/* ALU operation for Execute, subs, param1 - param2. */
int subOp (int param1, int param2) {
    return param1 - param2;
}

/* ALU operation for Execute, param1 << param2. */
int sllOp (int param1, int param2) {
    return param1 << param2;
}

/* ALU operation for Execute, param1 >> param2. */
int srlOp (int param1, int param2) {
    return param1 >> param2;
}

/* ALU operation for Execute, param1 & param2. */
int andOp (int param1, int param2) {
    return param1 & param2;
}

/* ALU operation for Execute, param1 | param2. */
int orOp (int param1, int param2) {
    return param1 | param2;
}

/* ALU operation for Execute, param1 < param2: 1 else 0. */
int sltOp (int param1, int param2) {
    return param1 < param2;
}

// /* ALU operation for Execute, param1 << 16. */
// int luiOp (int param1, int param2) {
//     return param1 << 16;
// }

