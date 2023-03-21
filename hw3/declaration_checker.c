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

int scope_size()
{
}

void initSymbolTable()
{
    symbols = 0;
    symbolTable = malloc(MAX_SCOPE_SIZE * sizeof(id_attrs));
}

// if it's in the table: return 1
int checkSymbolTable(AST *declaration)
{
    file_location key = declaration->file_loc;

    for (int i = 0; i < symbols; i++)
        if (strcmp(symbolTable[i].file_loc.filename, key.filename) == 0)
            return 1;

    return 0;
}

void checkDeclaration(AST *progast)
{
    initSymbolTable();

    checkConsDecls(progast->data.program.cds);

    checkVarDecls(progast->data.program.vds);

    checkStmt(progast->data.program.stmt);
}

void checkConsDecls(AST_list cds)
{
    while (!ast_list_is_empty(cds))
    {
        AST *cd = ast_list_first(cds);
        const char *name = cd->data.var_decl.name;
        file_location floc = cd->file_loc;

        // produces error message if symbol table already contains the same declared name
        if (checkSymbolTable(ast_list_first(cds)) == 1)
        {
            general_error(floc,
                          "%s \"%s\" is already declared as a %s",
                          vt2str(vt), name, );
        }
        else
        {
            symbolTable[symbols] = create_id_attrs(cds->file_loc, id_kind k, scope_size());
            cds = ast_list_rest(cds);
        }
    }

    // gary leavins' code for this step with variables checker
    const char *name = vd->data.var_decl.name;
    var_type vt = vd->data.var_decl.vt;
    file_location floc = vd->file_loc;

    // if the name has already been declared...
    id_attrs *attrs = scope_lookup(name);
    if (attrs != NULL)
    {
        // ... produce an error message,
        general_error(floc,
                      "%s \"%s\" is already declared as a %s",
                      vt2str(vt), name, vt2str(attrs->vt));
    }
    else
    {
        // otherwise add the declaration to the symbol table
        scope_insert(name,
                     create_id_attrs(floc, vt, scope_size()));
    }
}

void checkVarDecls(AST_list vds)
{
    while (!ast_list_is_empty(vds))
    {
        // if it's in the symbol table
        if (checkSymbolTable(ast_list_first(vds)))
        {
            // ERROR
        }

        // add it into symbolTable

        // increment
        vds = ast_list_rest(vds);
    }
}

void checkStmtDecl(AST *stmt)
{
    // more gary code
    switch (stmt->type_tag)
    {
    case assign_ast:
        checkAssignStmt(stmt);
        break;
    case begin_ast:
        checkBeginStmt(stmt);
        break;
    case if_ast:
        checkIfStmt(stmt);
        break;
    case read_ast:
        checkReadStmt(stmt);
        break;
    case write_ast:
        checkWriteStmt(stmt);
        break;
    default:
        bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
        break;
    }
}


void checkAssignStmt(AST *stmt){
    checkIdent(stmt->file_loc, stmt->data.assign_stmt.name);
    checkExpr(stmt->data.assign_stmt.exp);
}

void checkBeginStmt(AST *stmt);

void checkIfStmt(AST* stmt);

void checkReadStmt(AST *stmt);

void checkWriteStmt(AST *stmt);