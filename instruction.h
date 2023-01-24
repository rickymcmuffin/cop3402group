#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

typedef struct
{
     int op; // opcode
     int m;  // M
} Instruction;

Instruction **readInstructions(char *fileLoc);

#endif