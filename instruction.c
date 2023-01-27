#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "vm.h"

// returns an array of type Instruction containing all of the instructions
// and an integer value of the length
Instruction **readInstructions(char *fileLoc, int *i)
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
     for (*i = 0; fscanf(fp, "%d %d", &op, &m) != EOF; *i+=1)
     {
          IM[*i] = malloc(sizeof(Instruction));
          IM[*i]->op = op;
          IM[*i]->m = m;
     }
     fclose(fp);

     return IM;
}

