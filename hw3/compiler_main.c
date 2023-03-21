#include <stdio.h>
#include "parser.h"
#include "unparser.h"
// #include "declaration_checker.h"
 
int main(int argc, char **argv)
{

	if (argc < 2)
	{
		printf("ERROR: ok dude, this program requires a file as an input!\n");
		return 1;
	}

	AST *ast = parser_open(argv[1]);
	unparseProgram(stdout, ast);

	// buildSymbolTable(ast);
	return 0;
}