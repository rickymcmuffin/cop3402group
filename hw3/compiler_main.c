#include <stdio.h>
#include "parser.h"

int main(int argc, char ** argv)
{
	if (argc < 2)
    {
        printf("ERROR: ok dude, this program requires a file as an input!\n");
        return 0;       
    }

	AST *ast = parser_open(argv[1]);
}