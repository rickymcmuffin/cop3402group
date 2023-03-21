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
	symbolTable = malloc(MAX_SCOPE_SIZE * sizeof(symbolNode));
}

// Return (a pointer to) the attributes
// of the given name
// or NULL if it is not declared
id_attrs *checkSymbolTable(char *name)
{
	for (int i = 0; i < symbols; i++)
		if (strcmp(symbolTable[i].name, name) == 0)
			return &symbolTable[i].attrs;

	return NULL;
}

void insertSymbolTable(char *name, id_attrs *attrs)
{
	if (checkSymbolTable(name))
	{
		if (symbols >= MAX_SCOPE_SIZE)
		{
			bail_with_error("ERROR: exceeded MAX_SCOPE_SIZE");
		}

		symbolTable[symbols].name = name;
		symbolTable[symbols].attrs = attrs;
		symbols++;
	}
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
		checkConsDecl(ast_list_first(cds));
		
			// add to symbol table
			cds = ast_list_rest(cds);
	}

	// gary leavins' code for this step with variables checker
	char *name = vd->data.var_decl.name;
	var_type vt = vd->data.var_decl.vt;
	file_location floc = vd->file_loc;

	// if the name has already been declared...
	id_attrs *attrs = scope_lookup(name);
	if (attrs != NULL)
	{
		// ... produce an error message,
		general_error(floc,
				    "variable \"%s\" is already declared as a %s", name, vt2str(attrs->vt));
	}
	else
	{
		// otherwise add the declaration to the symbol table
		scope_insert(name,
				   create_id_attrs(floc, vt, scope_size()));
	}
}

void checkConsDecl(AST *cd){

	const char *name = cd->data.var_decl.name;
		file_location floc = cd->file_loc;

		// produces error message if symbol table already contains the same declared name
		if (checkSymbolTable(ast_list_first(cds)) == 1)
		{
			general_error(floc, "constant \"%s\" is already declared as a constant", name);
		}
}

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
	char *name = vd->data.var_decl.name;
	file_location floc = vd->file_loc;

	// if it's in the symbol table
	if (checkSymbolTable(name) == NULL)
	{
		// ... produce an error message,
		general_error(floc,
				    "variable \"%s\" is already declared as a %s", name, vt2str(attrs->vt));
	}

	// add it into symbolTable
	insertSymbolTable(name, create_id_attrs(floc, id_kind k, unsigned int ofst)); // <-------------------------------------------------------------------------- not done pls fix
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
		checkStmtDecl(ast_list_first(stmtList));
		stmtList = ast_list_rest(stmtList);
	}
}

void checkIfStmt(AST *stmt)
{
	checkCond(stmt->data.if_stmt.cond);
	checkStmt(stmt->data.if_stmt.thenstmt);
	checkStmt(stmt->data.if_stmt.elsestmt);
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
	general_error(cond->file_loc, "Wrong AST type???");
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
		general_error(exp->file_loc, "Wrong AST type???");
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

void checkIdent(file_location loc, char *name)
{
	if (checkSymbolTable(name) == NULL)
	{
		general_error(loc, "identifer \"%s\" is not declared!", name);
	}
}