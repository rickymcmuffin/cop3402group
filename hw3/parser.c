#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

void parser_open(char*fileName);
void eat(token_type tokenName);

AST* parseProgram();
void parseBlock();

FILE *fp;
token ret;

void parser_open(char *fileName)
{
    fp = fopen(fileName, "r");
}

void eat(token_type tokenName)
{
    // if (ret.typ == tokenName)
    // {
    //     ret = lexer_next();
    // }
}

AST* parseProgram()
{
    // parseBlock();

    return NULL;
}

void parseBlock();

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("ERROR: ok dude, this program requires a file as an input!\n");
        return 0;       
    }

    parser_open(argv[1]);

    if (fp == NULL)
    {
        printf("ERROR: woah there PAL, you put in an invalid file name! Try again with one that works :)\n");
        return 0;       
    }
        
}