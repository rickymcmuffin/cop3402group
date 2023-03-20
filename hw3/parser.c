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
	token *old = ret;
	if (ret->typ == tokenName)
		*ret = lexer_next();

	return *old;
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

AST_list parseConstDeclLine(){

	AST_list cds;

	eat(constsym);

	cds = ast_list_singleton(parseConstDecl());
	
	while(1){
		if(ret->typ = semisym){
			eat(semisym);
			return cds;
		}
		eat(commasym);
		ast_list_splice(cds, parseConstDecl());
	}

}

AST *parseConstDecl()
{
	AST *cd;
	token iden = eat(identsym);
	eat(eqsym);
	token num = eat(numbersym);

	cd = ast_const_def(iden, iden.text, num.value);
	return cd;
}

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

AST_list parseVarDeclLine(){

	AST_list vds;

	eat(varsym);

	vds = ast_list_singleton(parseVarDecl());
	
	while(1){
		if(ret->typ = semisym){
			eat(semisym);
			return vds;
		}
		eat(commasym);
		ast_list_splice(vds, parseVarDecl());
	}

}

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
// not done
AST *parseStmt()
{
	switch (ret->typ)
	{

	default:
		break;
	}
}

void parseSemiStmt();

void parseEmpty();

void parseCondition();

void parseRelOp();

void parseExpr();

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
	return ast_op_expr(*ret, )
}
