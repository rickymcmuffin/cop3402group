#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "utilities.h"

AST *parser_open(char *fileName);
void eat(token_type tokenName);

AST* parseProgram();
void parseBlock();

#endif