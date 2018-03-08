#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "decode.h"
#include "execute.h"

enum AluOperation {
    ADD_OP = 1,
    SUB_OP = 2,
    OR_OP = 3,
    AND_OP = 4,
    SLL_OP = 5,
    SRL_OP = 6,
    SLT_OP = 7
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
    logMsg("getParam1()");
       
    if(d->type == R)

        return getRFormatParam1(aluOperation, d);

    else if(d->type == I)

        return getIFormatParam1(aluOperation, d);

    return 0;
}

/* Returns the first parameter of the ALU operations for R format instructions. */
int getRFormatParam1 (int aluOperation, DecodedInstr* d) {
    logMsg("getRFormatParam1()");
       
    if(aluOperation == SLL_OP || aluOperation == SRL_OP)

        return mips.registers[d->regs.r.rt];
    
    else

        return mips.registers[d->regs.r.rs];
}

/* Returns the first parameter of the ALU operations for I format instructions. */
int getIFormatParam1 (int aluOperation, DecodedInstr* d) {
    logMsg("getIFormatParam1()");
       
    if(d->op == LUI_OPCODE)

        return d->regs.i.addr_or_immed;
    
    else

        return mips.registers[d->regs.i.rs];
}

/* Returns the second parameter of the ALU operations. */
int getParam2 (int aluOperation, DecodedInstr* d) {
    logMsg("getParam2()");
       
    if(d->type == R)

        return getRFormatParam2(aluOperation, d);

    else if(d->type == I)

        return getIFormatParam2(aluOperation, d);

    return 0;
}

/* Returns the second parameter of the ALU operations for R format instructions. */
int getRFormatParam2 (int aluOperation, DecodedInstr* d) {
    logMsg("getRFormatParam2()");
       
    if(aluOperation == SLL_OP || aluOperation == SRL_OP)

        return d->regs.r.shamt;
    
    else

        return mips.registers[d->regs.r.rt];
}

/* Returns the second parameter of the ALU operations for I format instructions. */
int getIFormatParam2 (int aluOperation, DecodedInstr* d) {
    logMsg("getIFormatParam2()");
       
    if(d->op == LUI_OPCODE)

        return 16;
    
    else if(aluOperation == SUB_OP)

        return mips.registers[d->regs.i.rt];
    
    else

        return d->regs.i.addr_or_immed;
}

/* Executes the ALU operation and returns the value. */
int executeOperation (int aluOperation, int param1, int param2) {
    if(aluOperation == ADD_OP)

        return addOp(param1, param2);

    else if(aluOperation == SUB_OP)

        return subOp(param1, param2);

    else if(aluOperation == SLL_OP)

        return sllOp(param1, param2);

    else if(aluOperation == SRL_OP)

        return srlOp(param1, param2);

    else if(aluOperation == AND_OP)

        return andOp(param1, param2);

    else if(aluOperation == OR_OP)

        return orOp(param1, param2);

    else if(aluOperation == SLT_OP)

        return sltOp(param1, param2);

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

