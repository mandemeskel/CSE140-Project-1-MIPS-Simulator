#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "helper.h"
#include "decode.h"


/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
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
int sltbOp (int param1, int param2) {
  return param1 < param2;
}

/* ALU operation for Execute, param1 << 16. */
int luiOp (int param1, int param2) {
  return param1 << 16;
}

