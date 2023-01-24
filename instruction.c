#include <stdio.h>

#define MAX_CODE_LENGTH 512
#define MAX_STACK_HEIGHT 2048

typedef struct
{
     int op; // opcode
     int m;  // M
} Instruction;

Instruction **readInstructions(char *fileLoc)
{
     FILE *fp;
     fp = fopen(fileLoc, "r");
     if (fp == NULL)
     {
          fprintf(stderr, "Error: file not found\n");
          return NULL;
     }

     Instruction **IM = calloc(MAX_CODE_LENGTH, sizeof(Instruction *));

     int op, m;
     for (int i = 0; fscanf(fp, "%d %d", &op, &m) != EOF; i++)
     {
          IM[i] = malloc(sizeof(Instruction));
          IM[i]->op = op;
          IM[i]->m = m;
     }
     fclose(fp);

     return IM;
}