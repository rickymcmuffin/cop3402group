#include <stdio.h>
#include "instruction.h"

// we don't need push and pop if we use the pseudo code
// in the pdf
void push(Instruction **IS, Instruction *iPtr)
{
}

Instruction *pop(Instruction **IS)
{
}

void runProgram(Instruction **IM)
{
	int programC = 0, baseP = 0, stackP = 0;
	int halt = 0, debug = 1;

	// Array implementation of stack holding integers
	int *stack = calloc(MAX_STACK_HEIGHT, sizeof(int));

	printf("Tracing ...\n");
	while (!halt)
	{
		switch (IM[programC]->op)
		{
		case 1: // LIT
			stack[stackP] = IM[programC]->m;
			stackP = stackP + 1;
			break;
		case 2: // RTN
			programC = stack[stackP - 1];
			baseP = stack[stackP - 2];
			stackP = stackP - 2;
			break;
		case 3: // CAL
			stack[stackP] = baseP;
			stack[stackP + 1] = programC;
			baseP = stackP;
			stackP = stackP + 2;
			programC = IM[programC]->m;
			break;
		case 4: // POP
			stackP = stackP - 1;
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

void printInstruction(Instruction **IM, int PC){
	printf("==> addr: %d	%s	%d\n", PC, opToString(IM[PC]->op), IM[PC]->m);
}

void printDebug(int *stack, int PC, int BP, int SP){
	printf("PC: %d BP: %d SP: %d\n", PC, BP, SP);
	printf("stack: ");
	for(int i = 0; i < SP; i++){
		printf("S[%d]: %d", i, stack[i]);
	}
	printf("\n");
}

char *opToString(int op){
	char names[27][5] = {"LIT", "RTN", "CAL", "POP", "PSI", "PRM", "STO", "INC", "JMP",
                         "JPC", "CHO", "CHI", "HLT", "NDB", "NEG", "ADD", "SUB", "MUL",
                         "DIV", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "PSP"};
	return names[op - 1];
}