#ifndef _GEN_CODE_H
#define _GEN_CODE_H
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
	label l;
} procedure;



// boolean (0 or 1), true if list is empty
int procedureListEmpty(procedureList list);

// adds a procedure to the end of procedureList
procedureList procedureListAddToEnd(procedureList list, procedure *p);

// returns a code_seq from a procedureList
code_seq procedureListToCode(procedureList pl);

#endif