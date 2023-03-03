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
	if (ret->typ == tokenName) 
        *ret = lexer_next();

    return *ret;
}

AST* parseProgram()
{
	AST* prog;
    	AST_list cds = parseConstDecls();
    	AST_list vds = parseVarDecls();
    	AST* stmt = parseStmt();
	
	prog = ast_program(ret->filename, lexer_line(), lexer_column(), cds, vds, stmt); 
	eat(periodsym);
	return prog;
}

AST_list parseConstDecls()
{
    AST_list cds = malloc(100 * sizeof(AST_list));
    int i = 0;
    
    while (1)
    {
        if (ret->typ != constsym)
            return cds;
        
        cds[i] = parseConstDecl();
        i++;
    }
}

AST * parseConstDecl(){
	AST * cd;
	token cons = eat(constsym);
	token iden = eat(identsym);
	eat(eqsym);
	token num = eat(numbersym);
	
	cd = ast_const_def(cons, iden.text, num.value);
	return cd;
}


AST_list parseVarDelcs()
{
	AST_list vds = malloc(100 * sizeof(AST_list));
    int i = 0;
    
    while (1)
    {
        if (ret->typ != varsym)
            return vds;
        
        vds[i] = parseVarDecl();
        i++;
    }
}

AST *parseVarDelc()
{
    eat(varsym);
    eat(identsym);
    return ast_var_decl(*ret, ret->text);
}

void parseCommmaIdents(){
	eat(commasym);
	eat(identsym);
}
// not done
AST *parseStmt(){
	switch(ret->typ){
		
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
    switch(ret->typ)
    {
        case identsym:
            eat(identsym);
            break;
    }
   
    
}

AST* parseSign()
{
    switch(ret->typ)
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
    AST* expr = parseExpr();
    return ast_op_expr(*ret, )
}

