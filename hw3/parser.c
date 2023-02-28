#include "parser.h"

#include "ast.h"
#include "lexer.h"
#include "utilities.h"

FILE *fp;
token ret;

AST *parser_open(char *fileName)
{
    fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        bail_with_error("ERROR: woah there PAL, you put in an invalid file name! Try again with one that works :)\n");
        return 0;       
    }

    lexer_open(fileName);
    
    return parseProgram();
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