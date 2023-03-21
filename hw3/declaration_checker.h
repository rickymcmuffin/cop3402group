#include <stdlib.h>
#include <string.h>
#include "id_attrs.h"
#include "ast.h"
#include "utilities.h"
#define MAX_SCOPE_SIZE 4096

id_attrs *symbolTable;

// symbols is the current number of elements
int symbols;

void initSymbolTable();

int scope_size();

void checkConsDecls(AST_list cds);

void checkVarDecls(AST_list vds);

void checkStmtDecl(AST *stmt);

// checks the symbol table for duplicate declarations or identifiers w/o declarations
void checkDeclarations();