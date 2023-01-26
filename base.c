#include <stdio.h>
#include "instruction.h"

void printInstructions(Instruction **arr, int size)
{
    printf("Addr  OP    M\n");

    char names[27][5] = {"LIT", "RTN", "CAL", "POP", "PSI", "PRM", "STO", "INC", "JMP",
                         "JPC", "CHO", "CHI", "HLT", "NDB", "NEG", "ADD", "SUB", "MUL",
                         "DIV", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "PSP"};

    for (int i = 0; i < size; i++)
        printf("%-6d%-6s%d\n", i, names[arr[i]->op - 1], arr[i]->m);
}

int main(int argc, char **argv)
{
    printf("%s\n", argv[1]); // --------------------------------------------------------------- debug

    int len;
    Instruction **list = readInstructions(argv[1], &len); 

    // print out this list
    printInstructions(list, len);

    return 0;
}