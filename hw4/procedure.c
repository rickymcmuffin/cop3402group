#include "procedure.h"

procedure *procedureNew(code_seq block, label *label)
{
	procedure *ret = malloc(sizeof(procedure));
	ret->block = block;
	ret->label = label;
	ret->next = NULL;
	ret->pList = NULL;
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
	while (!procedureListEmpty(pl))
	{
		label_set(procedureListFirst(pl)->label, lineNumber);

		// adds the procedures first
		code_seq temp = procedureListToCodeHelper(pl->pList, lineNumber + 1);
		// adds the block after
		temp = code_seq_concat(temp, pl->block);

		lineNumber += code_seq_size(temp) + 1;

		ret = code_seq_concat(ret, temp);
	}
	// starts with jmp and ends with rtn
	int length = code_seq_size(ret);
	code_seq jmp = code_seq_singleton(code_jmp(length + 1));
	ret = code_seq_concat(jmp, ret);
	ret = code_seq_add_to_end(ret, code_rtn());
	return ret;
}

int procedureListIsEmpty(procedureList list)
{
	return (list == NULL);
}

// returns first element of procedureList
procedure *procedureListFirst(procedureList list)
{
	return list;
}

// returns next element of procedureList
procedure *procedureListNext(procedureList list)
{
	return list->next;
}

procedureList procedureListAddToEnd(procedureList list, procedure *p)
{
	if (procedureListEmpty(list))
		return NULL;

	procedure *temp = list;
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