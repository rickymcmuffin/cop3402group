#ifndef _PROCEDURE_H
#define _PROCEDURE_H
#include <stdlib.h>
#include "ast.h"
#include "code.h"

typedef struct procedures procedureS;

// list of procedures
typedef procedureS *procedureList;

// stores a procedure and it's address
typedef struct procedures {
	// procedureList inside of procedures so we can nest procedures
	procedureList pList;
	
	procedureS *next;
	code_seq block;
	label *label;
	
	AST *prodAST;
} procedureS;

// allocates a new procedure
extern procedureS *procedureNew(code_seq block, label *label, AST *ast);

// boolean (0 or 1), true if list is empty
extern int procedureListIsEmpty(procedureList list);

// returns first element of procedureList
extern procedureS *procedureListFirst(procedureList list);

// returns next element of procedureList
extern procedureS *procedureListNext(procedureList list);

// adds a procedure to the end of procedureList
extern procedureList procedureListAddToEnd(procedureList list, procedureS *p);

// returns a code_seq from a procedureList
extern code_seq procedureListToCode(procedureList pl);

#endif