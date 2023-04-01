#ifndef _DECLERATION_CHECKER_H 
#define _DECLERATION_CHECKER_H 

#include <stdio.h>
#include "token.h"
#include "file_location.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "id_attrs.h"
#include "ast.h"
#include "utilities.h"
#define MAX_SCOPE_SIZE 4096

typedef struct
{
    id_attrs *attrs;
    const char *name;
} symbolNode;


// checks the symbol table for duplicate declarations or identifiers w/o declarations
void checkDeclarations();

void initSymbolTable();

int scope_size();

id_attrs *checkSymbolTable(const char *name);

void insertSymbolTable(const char *name, id_attrs *attrs);

void checkDeclaration(AST *progast);

void checkConsDecls(AST_list cds);

void checkConsDecl(AST *cd);

void checkVarDecls(AST_list vds);

void checkVarDecl(AST *vd);

void checkStmt(AST *stmt);

void checkAssignStmt(AST *stmt);

void checkBeginStmt(AST *stmt);

void checkIfStmt(AST *stmt);

void checkWhileStmt(AST *stmt);

void checkReadStmt(AST *stmt);

void checkWriteStmt(AST *stmt);

void checkCond(AST *cond);

void checkExpr(AST *exp);

void checkBinExpr(AST *exp);

void checkOpExpr(AST *exp);

void checkIdent(file_location loc, const char *name);

#endif