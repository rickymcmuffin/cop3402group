#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED
#include "instruction.h"


void runProgram(Instruction **IM);
void push(Instruction **instStack, Instruction *iPtr);
Instruction* pop(Instruction **instStack);
char *opToString(int op);

#endif