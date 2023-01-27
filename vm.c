#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "vm.h"

// we don't need push and pop if we use the pseudo code
// in the pdf

int runProgram(Instruction **IM)
{
	char c;
	int programC = 0, baseP = 0, stackP = 0;
	int halt = 0, debug = 1;

	// Array implementation of stack holding integers
	int *stack = calloc(MAX_STACK_HEIGHT, sizeof(int));

	printf("Tracing ...\n");
	printDebug(stack, programC, baseP, stackP);
	while (!halt)
	{
		if (debug)
			printInstruction(IM, programC);
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
			if(stackP == 0){
				printf("Trying to pop an empty stack!\n");
				return 6;
			}
			stackP = stackP - 1;
			break;
		case 5: // PSI
			stack[stackP - 1] = stack[stack[stackP - 1]];
			break;
		case 6: // PRM
			stack[stackP] = stack[baseP - IM[programC]->m];
			stackP = stackP + 1;
			break;
		case 7: // STO
			stack[stack[stackP - 1] + IM[programC]->m] = stack[stackP - 2];
			stackP = stackP - 2;
			break;
		case 8: // INC
			stackP = stackP + IM[programC]->m;
			break;
		case 9: // JMP
			programC = stack[stackP - 1] - 1;
			stackP = stackP - 1;
			break;
		case 10: // JPC
			if (stack[stackP - 1] != 0)
			{
				programC = IM[programC]->m - 1;
			}
			stackP = stackP - 1;
			break;
		case 11: // CHO
			putc(stack[stackP - 1], stdout);
			stackP = stackP - 1;
			break;
		case 12: // CHI
			c = getc(stdin);

			if (c == EOF || ferror(stdin))
			{
				stack[stackP] = -1;
				stackP = stackP + 1;
			}

			stack[stackP] = c;
			stackP = stackP + 1;
			break;
		case 13: // HLT
			halt = 1;
			break;
		case 14: // NDB
			debug = 0;
			break;
		case 15: // NEG
			stack[stackP - 1] = stack[stackP - 1] * -1;
			break;
		case 16: // ADD
			stack[stackP - 2] = stack[stackP - 1] + stack[stackP - 2];
			stackP = stackP - 1;
			break;
		case 17: // SUB
			stack[stackP - 2] = stack[stackP - 1] - stack[stackP - 2];
			stackP = stackP - 1;
			break;
		case 18: // MUL
			stack[stackP - 2] = stack[stackP - 1] * stack[stackP - 2];
			stackP = stackP - 1;
			break;
		case 19: // DIV
			if (stack[stackP - 2] == 0)
			{
				fprintf(stdout, "Divisor is zero in DIV instruction!\n");
				return 4;
			}
			stack[stackP - 2] = stack[stackP - 1] / stack[stackP - 2];
			stackP = stackP - 1;

			break;
		case 20: // MOD
			if (stack[stackP - 2] == 0)
			{
				fprintf(stdout, "Modulus is zero in MOD instruction!\n");
				return 5;
			}
			stack[stackP - 2] = stack[stackP - 1] % stack[stackP - 2];
			stackP = stackP - 1;

			break;
		case 21: // EQL
			if (stack[stackP - 1] == stack[stackP - 2])
				stack[stackP - 2] = 1;
			else
				stack[stackP - 2] = 0;
			stackP = stackP - 1;
			break;
		case 22: // NEQ
			if (stack[stackP - 1] != stack[stackP - 2])
				stack[stackP - 2] = 1;
			else
				stack[stackP - 2] = 0;
			stackP = stackP - 1;
			break;
		case 23: // LSS
			if (stack[stackP - 1] < stack[stackP - 2])
				stack[stackP - 2] = 1;
			else
				stack[stackP - 2] = 0;
			stackP = stackP - 1;
			break;
		case 24: // LEQ
			if (stack[stackP - 1] <= stack[stackP - 2])
				stack[stackP - 2] = 1;
			else
				stack[stackP - 2] = 0;
			stackP = stackP - 1;
			break;
		case 25: // GTR
			if (stack[stackP - 1] > stack[stackP - 2])
				stack[stackP - 2] = 1;
			else
				stack[stackP - 2] = 0;
			stackP = stackP - 1;
			break;
		case 26: // GEQ
			if (stack[stackP - 1] >= stack[stackP - 2])
				stack[stackP - 2] = 1;
			else
				stack[stackP - 2] = 0;
			stackP = stackP - 1;
			break;
		case 27: // PSP
			stack[stackP] = stackP;
			stackP = stackP + 1;
			break;
		}
		programC++;
		if (debug)
			printDebug(stack, programC, baseP, stackP);
		if (!(0 <= baseP && baseP <= stackP && 0 <= stackP && stackP < MAX_STACK_HEIGHT))
		{
			printf("%d %d\n", baseP, stackP);
			fprintf(stderr, "BP/SP is out of bounds\n");
			return 2;
		}
		if (!(0 <= programC && programC < MAX_CODE_LENGTH))
		{
			fprintf(stderr, "PC out of bounds\n");
			return 3;
		}
	}
	return 0;
}

void printInstruction(Instruction **IM, int PC)
{
	char names[27][5] = {"LIT", "RTN", "CAL", "POP", "PSI", "PRM", "STO", "INC", "JMP",
					 "JPC", "CHO", "CHI", "HLT", "NDB", "NEG", "ADD", "SUB", "MUL",
					 "DIV", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "PSP"};
	printf("==> addr: %-6d%-6s%d\n", PC, names[IM[PC]->op - 1], IM[PC]->m);
}

void printDebug(int *stack, int PC, int BP, int SP)
{
	printf("PC: %d BP: %d SP: %d\n", PC, BP, SP);
	printf("stack: ");
	for (int i = 0; i < SP; i++)
	{
		printf("S[%d]: %d ", i, stack[i]);
	}
	printf("\n");
}
