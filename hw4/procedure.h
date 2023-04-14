#ifndef _PROCEDURE_H
#define _PROCEDURE_H
#include <stdlib.h>
#include "ast.h"
#include "code.h"

typedef struct procedures procedure;

// list of procedures
typedef procedure *procedureList;

// stores a procedure and it's address
typedef struct procedures {
	// procedureList inside of procedures so we can nest procedures
	procedureList pList;
	
	procedure *next;
	code_seq block;
	label *label;
} procedure;

// allocates a new procedure
procedureS *procedureNew(code_seq block, label *label);

// boolean (0 or 1), true if list is empty
int procedureListIsEmpty(procedureList list);

// returns first element of procedureList
procedure *procedureListFirst(procedureList list);

// returns next element of procedureList
procedure *procedureListNext(procedureList list);

// adds a procedure to the end of procedureList
procedureList procedureListAddToEnd(procedureList list, procedure *p);

// returns a code_seq from a procedureList
code_seq procedureListToCode(procedureList pl);

#endif