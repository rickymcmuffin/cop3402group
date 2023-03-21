#include "id_attrs.h"
#include "ast.h"

id_attrs *symbolTable;
// tableSize is the amount of spaces allocated in the symbol table
int tableSize;
// symbols is the current number of elements
int symbols;

void initSymbolTable();

void maybeReallocSymbolTable();

// builds the symbol table 
void buildSymbolTable(AST *progast);

// checks the symbol table for duplicate declarations or identifiers w/o declarations
void checkDeclarations();