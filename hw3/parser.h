#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <stdlib.h>


#include "ast.h"
#include "token.h"
#include "lexer.h"
#include "utilities.h"

AST *parser_open(char *fileName);
token eat(token_type tokenName);



void spliceButBetter(AST_list list, AST_list tail);
token *tokencopy(token *src);

AST* parseProgram();
void parseBlock();
AST_list parseConstDecls();
AST_list parseConstDeclLine();
AST *parseConstDecl();
AST_list parseVarDecls();
AST_list parseVarDeclLine();
AST *parseVarDecl();
AST *parseStmt();
AST *parseAssignStmt();
AST *parseBeginStmt();
AST *parseIfStmt();
AST *parseWhileStmt();
AST *parseReadStmt();
AST *parseWriteStmt();
AST *parseSkipStmt();
AST *parseCondition();
rel_op parseRelOp();
AST *parseExpr();
bin_arith_op parseAddSub();
AST *parseTerm();
bin_arith_op parseMultDiv();
AST* parseFactor();

#endif