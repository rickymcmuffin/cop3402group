#include "declaration_checker.h"

// --- global vars ---
symbolNode *symbolTable;
int symbols; // symbols is the current number of elements


// returns size of symbol table
int scope_size()
{
	return symbols;
}

// initializes symbol table and counter variable
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

// adds id_attrs to symbol table if it is unique and
// the symbol table is not full
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

// declaration checker for whole program
void checkDeclaration(AST *progast)
{
	initSymbolTable();

	checkConsDecls(progast->data.program.cds);

	checkVarDecls(progast->data.program.vds);

	checkStmt(progast->data.program.stmt);
}

// checks all constant declarations by traversing AST
void checkConsDecls(AST_list cds)
{

	while (!ast_list_is_empty(cds))
	{
		checkConsDecl(ast_list_first(cds));

		cds = ast_list_rest(cds);
	}
}

// checks for unique constant declarations
// outputs error message for duplicate declarations
// this function inserts unique constant declarations
// into the symbol table
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

// checks all variable declarations by traversing AST
void checkVarDecls(AST_list vds)
{
	while (!ast_list_is_empty(vds))
	{
		checkVarDecl(ast_list_first(vds));

		vds = ast_list_rest(vds);
	}
}

// This function checks for unique variable declarations
// if so, adds it as an id_attrs to symbol table
// if duplicate outputs error message
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

// checks for illegal statements using undeclared identifiers
// calls specific check functions for their respective statements
void checkStmt(AST *stmt)
{
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
		bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
		break;
	}
}

// calls the appropriate check functions for the grammar for assign statment
void checkAssignStmt(AST *stmt)
{
	checkIdent(stmt->file_loc, stmt->data.assign_stmt.name);
	checkExpr(stmt->data.assign_stmt.exp);
}

// loops through the AST list, checking all of the statments included
void checkBeginStmt(AST *stmt)
{
	AST_list stmtList = stmt->data.begin_stmt.stmts;
	while (!ast_list_is_empty(stmtList))
	{
		checkStmt(ast_list_first(stmtList));
		stmtList = ast_list_rest(stmtList);
	}
}

// calls the appropriate check functions according to the grammer for an if statement
void checkIfStmt(AST *stmt)
{
	checkCond(stmt->data.if_stmt.cond);
	checkStmt(stmt->data.if_stmt.thenstmt);
	checkStmt(stmt->data.if_stmt.elsestmt);
}

// calls the appropriate check functions according to the grammer for a while statement
void checkWhileStmt(AST *stmt)
{
	checkCond(stmt->data.while_stmt.cond);
	checkStmt(stmt->data.while_stmt.stmt);
}

// calls the appropriate check functions according to the grammer for a read statement
void checkReadStmt(AST *stmt)
{
	checkIdent(stmt->file_loc, stmt->data.read_stmt.name);
}

// calls the appropriate check functions according to the grammer for a write statement
void checkWriteStmt(AST *stmt)
{
	checkExpr(stmt->data.write_stmt.exp);
}

// checks for declaration errors in conditional statements
void checkCond(AST *cond)
{
	switch (cond->type_tag)
	{
	case odd_cond_ast:
		checkExpr(cond->data.odd_cond.exp);
		break;
	case bin_cond_ast:
		checkExpr(cond->data.bin_cond.leftexp);
		checkExpr(cond->data.bin_cond.rightexp);
		break;
	default:
		general_error(cond->file_loc, "Wrong AST type??? %d", cond->type_tag);
	}
}

// takes a statement AST and checks identifiers depending on case
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

// checks through both sides of the binary expression
void checkBinExpr(AST *exp)
{
	checkExpr(exp->data.bin_expr.leftexp);
	checkExpr(exp->data.bin_expr.rightexp);
}

// calls the appropriate check functions according to the grammer for an op
void checkOpExpr(AST *exp)
{
	checkExpr(exp->data.op_expr.exp);
}

// checks for duplicate identifiers. If found prints error message
void checkIdent(file_location loc, const char *name)
{
	if (checkSymbolTable(name) == NULL)
	{
		general_error(loc, "identifer \"%s\" is not declared!", name);
	}
}