#include "procedure.h"

int procedureListEmpty(procedureList list)
{
    return (list == NULL);
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
code_seq procedureListToCode(procedureList pl){
	if(pl == NULL){
		return NULL;
	}
	code_seq ret = code_seq_empty();
	while(!procedureListEmpty(pl)){
		// adds the procedures first
		ret = code_seq_concat(ret, procedureListToCode(pl->pList));
		// adds the block after
		ret = code_seq_concat(ret, pl->block);
	}
	// starts with jmp and ends with rtn
	int length = code_seq_size(ret);
	code_seq jmp = code_seq_singleton(code_jmp(length + 1));
	ret = code_seq_concat(jmp, ret);
	ret = code_seq_add_to_end(ret, code_rtn());
	return ret;
}