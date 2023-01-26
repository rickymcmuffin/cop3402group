#include <stdio.h>
#include "instruction.h"


void push(Instruction **IS, Instruction *iPtr)
{
    
}

Instruction* pop(Instruction **IS)
{
    
}

void runProgram(Instruction **IM)
{
    int programC = 0, baseP = 0, stackP = 0;

    // Array implementation of stack holding pointers to Instructions.
    Instruction **instStack = calloc(MAX_STACK_HEIGHT, sizeof(Instruction *));

    printf("Tracing ...\n");
    for (int i = 0; IM[i] != NULL; i++)
    {
        switch(IM[i]->op)
        {
            case 1: // LIT
            // insert logic that calls respective push/pop statements
            break;
            case 2: // RTN
            break;
            case 3: // CAL
            break;
            case 4: // POP
            break;
            case 5: // PSI
            break;
            case 6: // PRM
            break;
            case 7: // STO
            break;
            case 8: // INC
            break;
            case 9: // JMP
            break;
            case 10: // JPC
            break;
        }
    }
    

}