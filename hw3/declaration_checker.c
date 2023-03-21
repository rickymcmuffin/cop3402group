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

symbolNode *symbolTable;

// symbols is the current number of elements
int symbols;

int scope_size()
{
	return symbols;
}

void initSymbolTable()
{
	symbols = 0;
	symbolTable = malloc(MAX_SCOPE_SIZE * sizeof(symbolNode));
}

// Return (a pointer to) the attributes
// of the given name
// or NULL if it is not declared
id_attrs *checkSymbolTable(const char *name)
{
	for (int i = 0; i < symbols; i++)
		if (strcmp(symbolTable[i].name, name) == 0)
			return symbolTable[i].attrs;

	return NULL;
}

void insertSymbolTable(const char *name, id_attrs *attrs)
{
	if (checkSymbolTable(name))
	{
		if (symbols >= MAX_SCOPE_SIZE)
		{
			bail_with_error("ERROR: exceeded MAX_SCOPE_SIZE");
		}
	}
	symbolTable[symbols].name = name;
	symbolTable[symbols].attrs = attrs;
	symbols++;
}

void checkDeclaration(AST *progast)
{
	initSymbolTable();

	checkConsDecls(progast->data.program.cds);

	checkVarDecls(progast->data.program.vds);

	checkStmt(progast->data.program.stmt);
}

// conts are good
void checkConsDecls(AST_list cds)
{

	while (!ast_list_is_empty(cds))
	{
		checkConsDecl(ast_list_first(cds));

		// add to symbol table
		cds = ast_list_rest(cds);
	}
}

void checkConsDecl(AST *cd)
{

	// extract information from the AST
	const char *name = cd->data.const_decl.name;
	id_kind ct = constant;
	file_location floc = cd->file_loc;

	// if the name has already been declared...
	id_attrs *attrs = checkSymbolTable(name);
	if (attrs != NULL)
	{
		// ... produce an error message,
		general_error(floc,
				    "%s \"%s\" is already declared as a %s",
				    kind2str(ct), name, kind2str(attrs->kind));
	}
	else
	{
		// otherwise add the declaration to the symbol table
		insertSymbolTable(name,
					   create_id_attrs(floc, ct, scope_size()));
	}
}

// vars are good
void checkVarDecls(AST_list vds)
{
	while (!ast_list_is_empty(vds))
	{
		checkVarDecl(ast_list_first(vds));

		// increment
		vds = ast_list_rest(vds);
	}
}

void checkVarDecl(AST *vd)
{
	// extract information from the AST
	const char *name = vd->data.var_decl.name;
	const id_kind vt = variable;
	file_location floc = vd->file_loc;

	// if the name has already been declared...
	id_attrs *attrs = checkSymbolTable(name);
	if (attrs != NULL)
	{
		// ... produce an error message,
		general_error(floc,
				    "%s \"%s\" is already declared as a %s",
				    kind2str(vt), name, kind2str(attrs->kind));
	}
	else
	{
		// otherwise add the declaration to the symbol table
		insertSymbolTable(name,
					   create_id_attrs(floc, vt, scope_size()));
	}
}

void checkStmt(AST *stmt)
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
	case while_ast:
		checkWhileStmt(stmt);
		break;
	case read_ast:
		checkReadStmt(stmt);
		break;
	case write_ast:
		checkWriteStmt(stmt);
		break;
	case skip_ast:
		break;
	default:
		bail_with_error("Call to scope_check_stmt with an AST that is not a statement!2");
		break;
	}
}

void checkAssignStmt(AST *stmt)
{

	checkIdent(stmt->file_loc, stmt->data.assign_stmt.name);
	checkExpr(stmt->data.assign_stmt.exp);
}

void checkBeginStmt(AST *stmt)
{
	AST_list stmtList = stmt->data.begin_stmt.stmts;
	while (!ast_list_is_empty(stmtList))
	{
		checkStmt(ast_list_first(stmtList));
		stmtList = ast_list_rest(stmtList);
	}
}

void checkIfStmt(AST *stmt)
{
	checkCond(stmt->data.if_stmt.cond);
	checkStmt(stmt->data.if_stmt.thenstmt);
	checkStmt(stmt->data.if_stmt.elsestmt);
}

void checkWhileStmt(AST *stmt)
{
	checkCond(stmt->data.while_stmt.cond);
	checkStmt(stmt->data.while_stmt.stmt);
}

void checkReadStmt(AST *stmt)
{
	checkIdent(stmt->file_loc, stmt->data.read_stmt.name);
}

void checkWriteStmt(AST *stmt)
{
	checkExpr(stmt->data.write_stmt.exp);
}

void checkCond(AST *cond)
{
	if (cond->type_tag == odd_cond_ast)
	{
		checkExpr(cond->data.odd_cond.exp);
	}
	else if (cond->type_tag == bin_cond_ast)
	{
		checkExpr(cond->data.bin_cond.leftexp);
		checkExpr(cond->data.bin_cond.rightexp);
	}
	printf("%d", cond->type_tag == bin_cond_ast);
	general_error(cond->file_loc, "Wrong AST type???1 %d", cond->type_tag);
}

void checkExpr(AST *exp)
{
	switch (exp->type_tag)
	{
	case ident_ast:
		checkIdent(exp->file_loc, exp->data.ident.name);
		break;
	case bin_expr_ast:
		checkBinExpr(exp);
		break;
	case op_expr_ast:
		checkOpExpr(exp);
		break;
	case number_ast:
		break;
	default:
		general_error(exp->file_loc, "Wrong AST type???2");
		break;
	}
}

void checkBinExpr(AST *exp)
{
	checkExpr(exp->data.bin_expr.leftexp);
	checkExpr(exp->data.bin_expr.rightexp);
}

void checkOpExpr(AST *exp)
{
	checkExpr(exp->data.op_expr.exp);
}

void checkIdent(file_location loc, const char *name)
{

	if (checkSymbolTable(name) == NULL)
	{
		general_error(loc, "identifer \"%s\" is not declared!", name);
	}
}