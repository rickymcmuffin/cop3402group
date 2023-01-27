#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED
#include "instruction.h"


int runProgram(Instruction **IM);

void incrementStackP(int *sp, int n, int *error);
void decrementStackP(int *sp, int n, int *error);


void printDebug(int *stack, int PC, int BP, int SP);
void printInstruction(Instruction **IM, int PC);

#endif