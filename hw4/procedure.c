#include "procedure.h"

procedureS *procedureNew(code_seq block, label *label, AST *ast)
{
	procedureS *ret = malloc(sizeof(procedureS));
	ret->block = block;
	ret->label = label;
	ret->next = NULL;
	ret->pList = NULL;
	ret->prodAST = ast;
	return ret;
}

// lineNumber is the current size of the code
code_seq procedureListToCodeHelper(procedureList pl, int lineNumber)
{
	if (pl == NULL)
	{
		return NULL;
	}
	code_seq ret = code_seq_empty();
	while (!procedureListIsEmpty(pl))
	{
		label_set(procedureListFirst(pl)->label, lineNumber);

		// adds the procedures first
		code_seq temp = procedureListToCodeHelper(pl->pList, lineNumber + 1);
		// adds the block after
		temp = code_seq_concat(temp, pl->block);


		// gets the length of the constants and variables
		AST_list constants = procedureListFirst(pl)->prodAST->data.proc_decl.block->data.program.cds;
		AST_list variables = procedureListFirst(pl)->prodAST->data.proc_decl.block->data.program.vds;
		int varLength = ast_list_size(constants) + ast_list_size(variables);

		// removes the variables and constatnts from stack
		if (varLength > 0)
				temp = code_seq_add_to_end(temp, code_inc(-varLength));
		
		// adds the return call
		temp = code_seq_add_to_end(temp, code_rtn());

		lineNumber += code_seq_size(temp);

		ret = code_seq_concat(ret, temp);

		pl = procedureListNext(pl);
	}
	// starts with jmp and ends with rtn
	int length = code_seq_size(ret);
	code_seq jmp = code_seq_singleton(code_jmp(length + 1));
	ret = code_seq_concat(jmp, ret);
	
	return ret;
}

int procedureListIsEmpty(procedureList list)
{
	return (list == NULL);
}

// returns first element of procedureList
procedureS *procedureListFirst(procedureList list)
{
	return list;
}

// returns next element of procedureList
procedureS *procedureListNext(procedureList list)
{
	return (list == NULL) ? NULL : list->next;
}

procedureList procedureListAddToEnd(procedureList list, procedureS *p)
{
	if (procedureListIsEmpty(list))
		return p;

	procedureS *temp = list;
	while (temp->next != NULL)
		temp = temp->next;

	temp->next = p;

	return list;
}

// returns a code_seq from a procedureList
code_seq procedureListToCode(procedureList pl)
{
	return procedureListToCodeHelper(pl, 1);
}