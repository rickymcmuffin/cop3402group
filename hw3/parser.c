#include "parser.h"

#include "ast.h"
#include "lexer.h"
#include "utilities.h"

FILE *fp;
token *ret;

AST *parser_open(char *fileName)
{
	fp = fopen(fileName, "r");

	if (fp == NULL)
	{
		bail_with_error("ERROR: woah there PAL, you put in an invalid file name! Try again with one that works :)\n");
		return 0;
	}

	lexer_open(fileName);

	return parseProgram();
}

token eat(token_type tokenName)
{
	token old = *ret;
	if (ret->typ == tokenName)
		*ret = lexer_next();

	return old;
}

AST *parseProgram()
{
	AST *prog;
	AST_list cds = parseConstDecls();
	AST_list vds = parseVarDecls();
	AST *stmt = parseStmt();

	prog = ast_program(ret->filename, lexer_line(), lexer_column(), cds, vds, stmt);
	eat(periodsym);
	return prog;
}

// parses through all constants
AST_list parseConstDecls()
{
	AST_list cds = ast_list_empty_list();

	while (1)
	{
		if (ret->typ != constsym)
			return cds;

		ast_list_splice(cds, parseConstDeclLine());
	}
}

// parses through a single line of constant
AST_list parseConstDeclLine()
{

	AST_list cds;

	eat(constsym);

	cds = ast_list_singleton(parseConstDecl());

	while (1)
	{
		if (ret->typ = semisym)
		{
			eat(semisym);
			return cds;
		}
		eat(commasym);
		ast_list_splice(cds, parseConstDecl());
	}
}

// parses a single constant decleration
AST *parseConstDecl()
{
	AST *cd;
	token iden = eat(identsym);
	eat(eqsym);
	token num = eat(numbersym);

	cd = ast_const_def(iden, iden.text, num.value);
	return cd;
}

// parses through all variable declerations
AST_list parseVarDecls()
{
	AST_list vds = ast_list_empty_list();

	while (1)
	{
		if (ret->typ != varsym)
			return vds;

		ast_list_splice(vds, parseConstDeclLine());
	}
}

// parses a single line of variable decleration
AST_list parseVarDeclLine()
{

	AST_list vds;

	eat(varsym);

	vds = ast_list_singleton(parseVarDecl());

	while (1)
	{
		if (ret->typ = semisym)
		{
			eat(semisym);
			return vds;
		}
		eat(commasym);
		ast_list_splice(vds, parseVarDecl());
	}
}


// parses a signle variable decleration
AST *parseVarDecl()
{
	AST *vd;
	token iden = eat(identsym);

	vd = ast_var_decl(iden, iden.text);
	return vd;
}

void parseCommmaIdents()
{
	eat(commasym);
	eat(identsym);
}

// parses a statement
AST *parseStmt()
{
	switch (ret->typ)
	{
	case identsym:
		return parseAssignStmt();
		break;
	case beginsym:
		return parseBeginStmt();
		break;
	case ifsym:
		return parseIfStmt();
		break;
	case whilesym:
		return parseWhileStmt();
		break;
	case readsym:
		return parseReadStmt();
		break;
	case writesym:
		return parseWriteStmt();
		break;
	default:
		return parseSkipStmt();
		break;
	}
}

// parses the assigning variable statement
AST *parseAssignStmt()
{
	token iden = eat(identsym);
	eat(becomessym);
	AST *exp = parseExpr();

	return ast_assign_stmt(iden, iden.text, exp);
}

// parses the BeginEnd statement
AST *parseBeginStmt()
{
	token beg = eat(beginsym);
	AST_list stmts;

	stmts = ast_list_singleton(parseStmt());

	while(ret->typ != endsym){
		eat(semisym);
		ast_list_splice(stmts, parseStmt());
	}
	return ast_begin_stmt(beg, stmts);
	
}

// parses an if statement
AST *parseIfStmt(){
	token ifTok = eat(ifsym);
	AST *cond = parseCondition();
	eat(thensym);
	AST *thenstmt = parseStmt();
	eat(elsesym);
	AST *elsestmt = parseStmt();

	return ast_if_stmt(ifTok, cond, thenstmt, elsestmt);
}

// parses a while statement
AST *parseWhileStmt(){
	token whileTok = eat(whilesym);
	AST *cond = parseCondition();
	eat(dosym);
	AST *body = parseStmt();
	
	return ast_while_stmt(whileTok, cond, body);
}

// parses a read statement
AST *parseReadStmt(){
	token readTok = eat(readsym);
	token iden = eat(identsym);

	return ast_read_stmt(readTok, iden.text);
}

// parses a write statement
AST *parseWriteStmt(){
	token writeTok = eat(writesym);
	AST *exp = parseExpr();

	ast_write_stmt(writeTok, exp);
}

// parses a skip statement
AST *parseSkipStmt(){
	return ast_skip_stmt(eat(skipsym));
}

void parseSemiStmt();

void parseEmpty();

AST *parseCondition();

void parseRelOp();

AST *parseExpr();

void parseAddSubTerm();

void parseAddSub();

void parseTerm();

void parseMultDivFactor();

void parseMultDiv();

void parseFactor()
{
	switch (ret->typ)
	{
	case identsym:
		eat(identsym);
		break;
	}
}

AST *parseSign()
{
	switch (ret->typ)
	{
	case plussym:
		eat(plussym);
		break;
	case minussym:
		eat(minussym);
		break;
	default:
		token_type expected[] = {plussym, minussym};
		parse_error_unexpected(expected, 2, *ret);
	}
	AST *expr = parseExpr();
	//return ast_op_expr(*ret, )
}
