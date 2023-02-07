#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED
#include "instruction.h"


int runProgram(Instruction **IM);


void printDebug(int *stack, int PC, int BP, int SP);
void printInstruction(Instruction **IM, int PC);

#endif