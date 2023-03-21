#include "declaration_checker.h"


// add identifiers from cds to array
/*
    while (there are still identifiers)
    {
        identifier
        {
        check if identifiers array is full
            realloc
        check if constant declaration is already in array
            add to array

        }
        

    }

*/

// add identifiers from vds to array
/*
    while (there are still identifiers)
    {
        identifier
        {
        check if identifiers array is full
            realloc
        check if variable declaration is already in array
            add to array
            or return 1 (true)
        }
    }
*/

// int x;
// y = 1;



void initSymbolTable()
{
    tableSize = 100;
    symbols = 0;
    symbolTable = malloc(tableSize * sizeof(id_attrs));
}

void maybeReallocSymbolTable()
{
    if (symbols == tableSize)
    {
        symbolTable = realloc(symbolTable, tableSize * 2);
        tableSize *= 2;
    }
}

// if it's not in the table: return 1
int checkSymbolTable(AST declaration)
{
    file_location key = declaration.file_loc;
    for (int i = 0; i < symbols; i++)
    {
        if (strcmp(symbolTable[i].file_loc.filename, key.filename) == 1)
            return 1;
    }

    return 0;
}

void checkDeclaration(AST *progast)
{
    initSymbolTable();

    int cdsLen = sizeof(progast->data.program.cds->next) / sizeof(progast->data.program.cds->next[0]);
    checkConsDecls(progast->data.program.cds)
    
    // int vdsLen = sizeof(progast->data.program.vds->next) / sizeof(progast->data.program.vds->next[0]);
    // for (int i = 0; i < vdsLen; i++)
    // {
        
    //     // reallocates identifier array if full
    //     maybeReallocSymbolTable();
        
    //     progast->data.program.vds[i]
        
    // }
    checkVarDecls(progast->data.program.vds);
}

void checkConsDecls(AST_list cds)
{
    while (!ast_list_is_empty(cds))
    {
        // reallocates identifier array if full
        maybeReallocSymbolTable();

        if (checkSymbolTable(ast_list_first(cds)) == 1)
        {
            parse_error_general( should pass a token, 
            "constant \"%s\" is already declared as a variable", 
            ast_list_first(cds)->data.const_decl.name);
            continue;
        }
        symbolTable[symbols] = 
        cds = ast_list_rest(cds);
    }
}

void checkVarDecls(AST_list vds)
{
    while (!ast_list_is_empty(vds)) {
        if (checkSymbolTable(ast_list_first(vds)))
        {
            // do something
        }

        vds = ast_list_rest(vds);
    }
}

