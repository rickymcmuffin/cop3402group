#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

#define MAX_CODE_LENGTH 512
#define MAX_STACK_HEIGHT 2048

typedef struct
{
     int op; // opcode
     int m;  // M
} Instruction;

Instruction **readInstructions(char *fileLoc, int *i);
void printInstructions(Instruction **IM, int size);

#endif