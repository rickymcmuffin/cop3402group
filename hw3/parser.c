#include "parser.h"

FILE *fp;
token *currentTok;

AST *parser_open(char *fileName)
{
	FILE *filep = fopen(fileName, "r");

	if (filep == NULL)
	{
		bail_with_error("ERROR: woah there PAL, you put in an invalid file name! Try again with one that works :)\n");
		return 0;
	}

	lexer_open(fileName);

	return parseProgram();
}

token eat(token_type tokenName)
{
	token old = *currentTok;
	if (currentTok->typ == tokenName)
		*currentTok = lexer_next();

	return old;
}

AST *parseProgram()
{
	AST *prog;
	AST_list cds = parseConstDecls();
	AST_list vds = parseVarDecls();
	AST *stmt = parseStmt();

	prog = ast_program(currentTok->filename, lexer_line(), lexer_column(), cds, vds, stmt);
	eat(periodsym);
	return prog;
}

// parses through all constants
AST_list parseConstDecls()
{
	AST_list cds = ast_list_empty_list();

	while (1)
	{
		if (currentTok->typ != constsym)
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
		if (currentTok->typ == semisym)
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
		if (currentTok->typ != varsym)
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
		if (currentTok->typ == semisym)
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
	switch (currentTok->typ)
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

	while (currentTok->typ != endsym)
	{
		eat(semisym);
		ast_list_splice(stmts, parseStmt());
	}
	return ast_begin_stmt(beg, stmts);
}

// parses an if statement
AST *parseIfStmt()
{
	token ifTok = eat(ifsym);
	AST *cond = parseCondition();
	eat(thensym);
	AST *thenstmt = parseStmt();
	eat(elsesym);
	AST *elsestmt = parseStmt();

	return ast_if_stmt(ifTok, cond, thenstmt, elsestmt);
}

// parses a while statement
AST *parseWhileStmt()
{
	token whileTok = eat(whilesym);
	AST *cond = parseCondition();
	eat(dosym);
	AST *body = parseStmt();

	return ast_while_stmt(whileTok, cond, body);
}

// parses a read statement
AST *parseReadStmt()
{
	token readTok = eat(readsym);
	token iden = eat(identsym);

	return ast_read_stmt(readTok, iden.text);
}

// parses a write statement
AST *parseWriteStmt()
{
	token writeTok = eat(writesym);
	AST *exp = parseExpr();

	return ast_write_stmt(writeTok, exp);
}

// parses a skip statement
AST *parseSkipStmt()
{
	return ast_skip_stmt(eat(skipsym));
}

void parseEmpty();

AST *parseCondition()
{
	if (currentTok->typ == oddsym)
	{
		token oddTok = eat(oddsym);
		AST *exp = parseExpr();
		return ast_odd_cond(oddTok, exp);
	}

	token expTok = *currentTok;
	AST *e1 = parseExpr();
	rel_op relop = parseRelOp();
	AST *e2 = parseExpr();

	return ast_bin_cond(expTok, e1, relop, e2);
}

rel_op parseRelOp()
{
	switch (currentTok->typ)
	{
	case eqsym:
		eat(eqsym);
		return eqop;
		break;
	case neqsym:
		eat(neqsym);
		return neqop;
		break;
	case lessym:
		eat(lessym);
		return ltop;
		break;
	case leqsym:
		eat(leqsym);
		return leqop;
		break;
	case gtrsym:
		eat(gtrsym);
		return gtop;
		break;
	default:
		eat(geqsym);
		return (geqop);
		break;
	}
}

AST *parseExpr()
{
	token firstTok = *currentTok;
	AST *e1 = parseTerm();

	while (currentTok->typ == plussym || currentTok->typ == minussym)
	{
		bin_arith_op arith = parseAddSub();

		AST *e2 = parseTerm();

		e1 = ast_bin_expr(firstTok, e1, arith, e2);

	}

	return e1;
}

bin_arith_op parseAddSub()
{
	if (currentTok->typ == plussym)
	{
		eat(plussym);
		return addop;
	}
	eat(minussym);
	return subop;
}

AST *parseTerm()
{
	token firstTok = *currentTok;
	AST *e1 = parseFactor();

	while (currentTok->typ == multsym || currentTok->typ == divsym)
	{
		bin_arith_op arith = parseMultDiv();

		AST *e2 = parseFactor();

		e1 = ast_bin_expr(firstTok, e1, arith, e2);

	}

	return e1;

}


bin_arith_op parseMultDiv()
{
	if (currentTok->typ == multsym)
	{
		eat(multsym);
		return multop;
	}
	eat(divsym);
	return divop;
}

AST* parseFactor()
{
	switch (currentTok->typ)
	{
	case identsym:
		token i = eat(identsym);
		return ast_ident(i, i.text);
		break;
	case plussym:
		eat(plussym);
		token p = eat(numbersym);
		return ast_number(p, p.value);
		break;
	case minussym:
		eat(minussym);
		token m = eat(numbersym);
		return ast_number(m, -m.value);
		break;
	case numbersym:
		token n = eat(numbersym);
		return ast_number(n, n.value);
		break;
	default:
		eat(lparensym);
		AST *exp = parseExpr();
		eat(rparensym);
		return exp;
		break;

	}
}
