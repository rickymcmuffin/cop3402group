#include "parser.h"

FILE *filep;
token *currentTok;

// why wouldn't you make splice work like this ???
void spliceButBetter(AST_list list, AST_list tail){
	ast_list_last_elem(list)->next = tail;
}

// returns a copy of a token in parameter
token *tokencopy(token *src)
{

	token *ret;
	ret = malloc(sizeof(token));
	ret->text = calloc(MAX_IDENT_LENGTH + 1, sizeof(char));

	ret->filename = src->filename;
	ret->typ = src->typ;
	strcpy(ret->filename, src->filename);
	ret->line = src->line;
	ret->column = src->column;
	strcpy(ret->text, src->text);
	ret->value = src->value;

	return ret;
}

// runs the parser and return the completed AST
AST *parser_open(char *fileName)
{

	filep = fopen(fileName, "r");

	if (filep == NULL)
	{
		bail_with_error("ERROR: woah there PAL, you put in an invalid file name! Try again with one that works :)\n");
		return 0;
	}

	lexer_open(fileName);

	currentTok = malloc(sizeof(token));
	*currentTok = lexer_next();
	return parseProgram();
}

token eat(token_type tokenName)
{
	token old = *currentTok;
	if (currentTok->typ == tokenName)
		*currentTok = lexer_next();
	else
	{
		parse_error_unexpected(&tokenName, 1, *currentTok);
	}
	return old;
}

// returns full program AST
AST *parseProgram()
{
	token *start = tokencopy(currentTok);
	AST *prog;
	AST_list cds = parseConstDecls();
	AST_list vds = parseVarDecls();
	AST *stmt = parseStmt();

	//fflush(stdout);
	//printf("%s\n", ttyp2str(currentTok->typ));
	prog = ast_program(start->filename, start->line, start->column, cds, vds, stmt);
	eat(periodsym);
	eat(eofsym);

	free(start);
	return prog;
}

// parses through all constants
AST_list parseConstDecls()
{
	AST_list cds = ast_list_empty_list();

	while (currentTok->typ == constsym)
	{
		if (ast_list_is_empty(cds))
		{
			cds = ast_list_singleton(parseConstDeclLine());
		}
		else
		{
			spliceButBetter(cds, parseConstDeclLine());
		}
	}
	return cds;
}

// parses through a single line of constant
AST_list parseConstDeclLine()
{

	AST_list cds;

	eat(constsym);

	cds = ast_list_singleton(parseConstDecl());

	while (currentTok->typ != semisym)
	{
		eat(commasym);
		spliceButBetter(cds, parseConstDecl());
	}
	eat(semisym);
	return cds;
}

// parses a single constant decleration
AST *parseConstDecl()
{
	AST *cd;
	token *iden = tokencopy(currentTok);
	eat(identsym);
	eat(eqsym);
	token *num = tokencopy(currentTok);
	eat(numbersym);

	cd = ast_const_def(*iden, iden->text, num->value);
	return cd;
}

// parses through all variable declerations
AST_list parseVarDecls()
{
	AST_list vds = ast_list_empty_list();

	while (currentTok->typ == varsym)
	{
		if (ast_list_is_empty(vds))
		{
			vds = ast_list_singleton(parseVarDeclLine());
		}
		else
		{
			spliceButBetter(vds, parseVarDeclLine());
		}
	}
	return vds;
}

// parses a single line of variable decleration
AST_list parseVarDeclLine()
{

	AST_list vds;

	eat(varsym);

	vds = ast_list_singleton(parseVarDecl());

	while (currentTok->typ != semisym)
	{
		eat(commasym);
		spliceButBetter(vds, parseVarDecl());
	}
	eat(semisym);
	return vds;
}

// parses a signle variable decleration
AST *parseVarDecl()
{
	AST *vd;
	token *iden = tokencopy(currentTok);
	eat(identsym);
	vd = ast_var_decl(*iden, iden->text);

	free(iden);
	return vd;
}


// parses a statement
AST *parseStmt()
{
	// check which type of statment it is
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
	case skipsym:
		return parseSkipStmt();
		break;
	default:
		// error if none of them
		token_type expected[] = {identsym, beginsym, ifsym, whilesym, readsym, writesym, skipsym};
		parse_error_unexpected(expected, 7, *currentTok);
		return NULL;
		break;
	}
}

// parses the assigning variable statement
AST *parseAssignStmt()
{
	token *iden = tokencopy(currentTok);
	eat(identsym);
	eat(becomessym);
	AST *exp = parseExpr();

	return ast_assign_stmt(*iden, iden->text, exp);
}

// parses the BeginEnd statement
AST *parseBeginStmt()
{
	token *beg = tokencopy(currentTok);
	eat(beginsym);
	AST_list stmts;

	stmts = ast_list_singleton(parseStmt());

	while (currentTok->typ == semisym)
	{
		
		eat(semisym);
		spliceButBetter(stmts, parseStmt());
	}
	eat(endsym);
	AST *begg = ast_begin_stmt(*beg, stmts);

	free(beg);
	return begg;
}

// parses an if statement
AST *parseIfStmt()
{
	token *ifTok = tokencopy(currentTok);
	eat(ifsym);
	AST *cond = parseCondition();
	eat(thensym);
	AST *thenstmt = parseStmt();
	eat(elsesym);
	AST *elsestmt = parseStmt();

	AST *ff = ast_if_stmt(*ifTok, cond, thenstmt, elsestmt);

	free(ifTok);
	return ff;
}

// parses a while statement
AST *parseWhileStmt()
{
	token *whileTok = tokencopy(currentTok);
	eat(whilesym);
	AST *cond = parseCondition();
	eat(dosym);
	AST *body = parseStmt();

	AST *wh = ast_while_stmt(*whileTok, cond, body);

	free(whileTok);
	return wh;
}

// parses a read statement
AST *parseReadStmt()
{
	token *readTok = tokencopy(currentTok);
	eat(readsym);
	token *iden = tokencopy(currentTok);
	eat(identsym);

	AST *readret = ast_read_stmt(*readTok, iden->text);
	free(readTok);
	free(iden);

	return readret;
}

// parses a write statement
AST *parseWriteStmt()
{
	token *writeTok = tokencopy(currentTok);
	eat(writesym);
	AST *exp = parseExpr();

	return ast_write_stmt(*writeTok, exp);
}

// parses a skip statement
AST *parseSkipStmt()
{
	token *sk = tokencopy(currentTok);
	eat(skipsym);
	AST *ip = ast_skip_stmt(*sk);

	free(sk);
	return ip;
}

// parses a condition
AST *parseCondition()
{
	// check if oddsym
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
	// check if any of relative operators
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
	case geqsym:
		eat(geqsym);
		return (geqop);
		break;
	default:
		// error if none of them
		token_type expected[] = {eqsym, neqsym, lessym, leqsym, gtrsym, geqsym};
		parse_error_unexpected(expected, 6, *currentTok);
		return eqsym;
	}
}

// parses expression
AST *parseExpr()
{
	token firstTok = *currentTok;
	AST *e1 = parseTerm();

	while (currentTok->typ == plussym || currentTok->typ == minussym)
	{
		bin_arith_op arith = parseAddSub();

		AST *e2 = parseTerm();

		// use left recursion	
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

// parses term
AST *parseTerm()
{
	token firstTok = *currentTok;
	AST *e1 = parseFactor();

	while (currentTok->typ == multsym || currentTok->typ == divsym)
	{
		bin_arith_op arith = parseMultDiv();

		AST *e2 = parseFactor();

		// use left recursion
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

AST *parseFactor()
{
	switch (currentTok->typ)
	{
	case identsym:
		token *i = tokencopy(currentTok);
		eat(identsym);
		AST *ir = ast_ident(*i, i->text);
		free(i);
		return ir;
		break;
	case plussym:
		eat(plussym);
		token *p = tokencopy(currentTok);
		eat(numbersym);
		AST *pr = ast_number(*p, p->value);
		free(p);
		return pr;
		break;
	case minussym:
		eat(minussym);
		token *m = tokencopy(currentTok);
		eat(numbersym);
		AST *mr = ast_number(*m, -m->value);
		free(m);
		return mr;
		break;
	case numbersym:
		token *n = tokencopy(currentTok);
		eat(numbersym);
		AST *nr = ast_number(*n, n->value);
		free(n);
		return nr;
		break;
	case lparensym:
		eat(lparensym);
		AST *exp = parseExpr();
		eat(rparensym);
		return exp;
		break;
	default:
		// error if none of them
		token_type expected[] = {identsym, plussym, minussym, numbersym, lparensym};
		parse_error_unexpected(expected, 5, *currentTok);
		return NULL;

	}
}
