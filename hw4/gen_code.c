/* $Id: gen_code_stubs.c,v 1.8 2023/03/29 15:42:08 leavens Exp leavens $ */
#include "utilities.h"
#include "gen_code.h"
// #include "symtab.h"

// Initialize the code generator
void gen_code_initialize()
{
	// 	symtab_initialize();
	// pList = NULL;

	// Replace the following with your implementation

	// bail_with_error("gen_code_initialize not implemented yet!");
}

// Generate code for the given AST
code_seq gen_code_program(AST *prog)
{
	//if(pList == NULL){ bail_with_error("pList is Null");}
	code_seq ret = code_seq_concat(gen_code_block(prog), code_hlt());

	procedureList pList = gen_code_procDecls(prog->data.program.pds);

	//if (pList == NULL)
		//bail_with_error("peeList is null");
	code_seq proc = procedureListToCode(pList);
	proc = code_seq_concat(proc, code_seq_singleton(code_inc(3)));

	//if(proc == NULL){
		//bail_with_error("proc is null");
	//}
	
	// adding the jump instruction to the start

	ret = code_seq_concat(proc, ret);

	code_seq_fix_labels(ret);
	return ret;
}

// generate code for blk
code_seq gen_code_block(AST *blk)
{
	code_seq ret = code_seq_empty();
	code_seq constDecls = gen_code_constDecls(blk->data.program.cds);
	code_seq varDecls = gen_code_varDecls(blk->data.program.vds);
	
	/*if(pList != NULL){
		// int length = code_seq_size(pList);
		int length = sizeof(pList)/sizeof();
		ret = code_seq_add_to_end(ret, code_jmp(length + 1)); 
		ret = code_seq_concat(ret, pList);
	}*/
	code_seq stmt = gen_code_stmt(blk->data.program.stmt);

	ret = code_seq_concat(ret, constDecls);
	ret = code_seq_concat(ret, varDecls);
	ret = code_seq_concat(ret, stmt);
	return ret;
}

// generate code for the declarations in cds
code_seq gen_code_constDecls(AST_list cds)
{
	code_seq constDecls = code_seq_empty();

	// loops through the constant decls
	while (!ast_list_is_empty(cds))
	{
		// adds a new constDecl to the end of the code seq
		constDecls = code_seq_concat(constDecls, gen_code_constDecl(ast_list_first(cds)));
		cds = ast_list_rest(cds);
	}

	return constDecls;
}

// generate code for the const declaration cd
code_seq gen_code_constDecl(AST *cd)
{
	return code_lit(cd->data.const_decl.num_val);
}

// Not checked
// generate code for the declarations in vds
code_seq gen_code_varDecls(AST_list vds)
{
	// iterates through linked list of vds calling gen_code_varDecl on each var AST
	code_seq varCodeSeq = code_seq_empty();
	while (!ast_list_is_empty(vds))
	{
		varCodeSeq = code_seq_concat(varCodeSeq, gen_code_varDecl(ast_list_first(vds)));
		vds = ast_list_rest(vds);
	}

	return varCodeSeq;
}

// generate code for the var declaration vd
code_seq gen_code_varDecl(AST *vd)
{
	// returns new code with instructions to allocate 1 local to the stack
	return code_inc(1);
}

/* PROCEDURE STRUCTURE
	procedures first with jump around them
	jump depends on total size of procedures (in instructions)

	0   JMP N (around the procedures' code)
	1   [code for procedures]
	...
	N   [code for the main block]

	To find distance to JMP:
	code_seq_size() will give the N for JMP instruction

	Nested procedure	
	procedure a;
		procedure b:
		begin;
			call a;
		end;
	begin
	call b;
	end.
	
	For nested procedures, use labels. 
	;;;;;
	JMP L;
	;;;;;
	;;;;;
	;;;;;
	Label L: ;;;;;
	;;;;;
	labels save the addy of the first line of the procedure
	have to do a second pass?


	there is a discussion post about procedures
*/ 

// generate code for the declarations in pds
procedureList gen_code_procDecls(AST_list pds)
{
	// iterates through linked list of pds calling gen_code_procDecl on each proc AST
	// code_seq procCodeSeq = code_seq_empty(); OLD CODE OLD CODE OLD CODE OLD CODE OLD CODE OLD CODE OLD CODE OLD CODE OLD CODE
	// while (!ast_list_is_empty(pds))
	// {
	// 	procCodeSeq = code_seq_concat(procCodeSeq, gen_code_varDecl(ast_list_first(pds)));
	// 	pds = ast_list_rest(pds);
	// }

	procedureList ret = NULL;
	while (!ast_list_is_empty(pds))
	{
		procedureS *p = gen_code_procDecl(pds);
		
		ret = procedureListAddToEnd(ret, p);
		pds = ast_list_rest(pds);
	}

	return ret;
}

// generate code for the procedure declaration pd
procedureS *gen_code_procDecl(AST *pd)
{
	// if (pd == NULL) bail_with_error("pd is null");
	//if (p == NULL) bail_with_error("filename: %s p is null", pd->file_loc);

	code_seq block = gen_code_block(pd->data.proc_decl.block);
	label *lab = pd->data.proc_decl.lab;
	procedureS* ret = procedureNew(block, lab, pd);

	if (ret == NULL) bail_with_error("p is null 222222");

	ret->pList = gen_code_procDecls(pd->data.proc_decl.block->data.program.pds);

	return ret;
}

// generate code for the statement
code_seq gen_code_stmt(AST *stmt)
{
	switch (stmt->type_tag)
	{
	case assign_ast:
		return gen_code_assignStmt(stmt);
	case call_ast:
		return gen_code_callStmt(stmt);
	case begin_ast:
		return gen_code_beginStmt(stmt);
	case if_ast:
		return gen_code_ifStmt(stmt);
	case while_ast:
		return gen_code_whileStmt(stmt);
	case read_ast:
		return gen_code_readStmt(stmt);
	case write_ast:
		return gen_code_writeStmt(stmt);
	case skip_ast:
		return gen_code_skipStmt(stmt);
	default:
		bail_with_error("Call to gen_code_stmt with an AST that is not a statement!");
		return code_seq_empty();
	}
}

// it's no longer wrong
// generate code for the statement
code_seq gen_code_assignStmt(AST *stmt)
{
	code_seq ret;

	AST *astIdent = stmt->data.assign_stmt.ident;

\
	code_seq fp = code_compute_fp(astIdent->data.ident.idu->levelsOutward);
	code_seq exp = gen_code_expr(stmt->data.assign_stmt.exp);

	int offset = astIdent->data.ident.idu->attrs->loc_offset;
	code *sto = code_sto(offset);

	ret = fp;
	ret = code_seq_concat(ret, exp);
	ret = code_seq_add_to_end(ret, sto);


	return ret;
}

// generate code for the statement
code_seq gen_code_callStmt(AST *stmt)
{
	label *lab = stmt->data.call_stmt.ident->data.ident.idu->attrs->lab;
	code *cal = code_cal(lab);
	return code_seq_singleton(cal);
}

// generate code for the statement
code_seq gen_code_beginStmt(AST *stmt)
{
	code_seq ret = code_seq_empty();
	AST_list stmts = stmt->data.begin_stmt.stmts;
	while (!ast_list_is_empty(stmts))
	{
		// ret = gen_code_concat(ret, gen_code_stmt(ast_list_first(stmts)));
		ret = code_seq_concat(ret, gen_code_stmt(ast_list_first(stmts)));
		stmts = ast_list_rest(stmts);
	}
	return ret;
}

// generate code for the statement
code_seq gen_code_ifStmt(AST *stmt)
{
	// generate all sequences
	code_seq cond = gen_code_cond(stmt->data.if_stmt.cond);
	code_seq s1 = gen_code_stmt(stmt->data.if_stmt.thenstmt);
	code_seq s2 = gen_code_stmt(stmt->data.if_stmt.elsestmt);

	// generate singles
	code *jpc = code_jpc(2);
	code *jmp1 = code_jmp(code_seq_size(s1) + 2);
	code *jmp2 = code_jmp(code_seq_size(s2) + 1);

	// put it all together
	code_seq ret = cond;
	ret = code_seq_add_to_end(ret, jpc);
	ret = code_seq_add_to_end(ret, jmp1);
	ret = code_seq_concat(ret, s1);
	ret = code_seq_add_to_end(ret, jmp2);
	ret = code_seq_concat(ret, s2);

	return ret;
}

// generate code for the statement
code_seq gen_code_whileStmt(AST *stmt)
{
	// generate sequences
	code_seq cond = gen_code_cond(stmt->data.while_stmt.cond);
	code_seq s = gen_code_stmt(stmt->data.while_stmt.stmt);

	// generate singles
	code *jpc = code_jpc(2);
	code *jmp1 = code_jmp(code_seq_size(s) + 2);
	code *jmp2 = code_jmp(-(code_seq_size(s) + code_seq_size(cond) + 2));

	// put it all together
	code_seq ret = cond;
	ret = code_seq_add_to_end(ret, jpc);
	ret = code_seq_add_to_end(ret, jmp1);
	ret = code_seq_concat(ret, s);
	ret = code_seq_add_to_end(ret, jmp2);

	return ret;
}

// generate code for the statement
code_seq gen_code_readStmt(AST *stmt)
{
	code_seq expr = gen_code_expr(stmt->data.read_stmt.ident);
	return code_seq_add_to_end(expr, code_chi());
}

// generate code for the statement
code_seq gen_code_writeStmt(AST *stmt)
{
	code_seq expr = gen_code_expr(stmt->data.write_stmt.exp);
	code_seq ret = code_seq_add_to_end(expr, code_cho());

	return ret;
}

// generate code for the statement
code_seq gen_code_skipStmt(AST *stmt)
{
	return code_seq_singleton(code_nop());
}

// generate code for the condition
code_seq gen_code_cond(AST *cond)
{
	switch (cond->type_tag)
	{
	case (odd_cond_ast):
		return gen_code_odd_cond(cond);

	case (bin_cond_ast):
		return gen_code_bin_cond(cond);
	default:
		bail_with_error("Call to gen_code_cond with an AST that is not a condition!");
		return code_seq_empty();
	}
}

// generate code for the condition
code_seq gen_code_odd_cond(AST *cond)
{
	code_seq ret;
	ret = gen_code_expr(cond->data.odd_cond.exp);

	ret = code_seq_add_to_end(ret, code_lit(2));
	ret = code_seq_add_to_end(ret, code_mod());

	return ret;
}

// generate code for the condition
code_seq gen_code_bin_cond(AST *cond)
{
	// generate code for both expressions
	code_seq ret;
	ret = gen_code_expr(cond->data.bin_cond.leftexp);
	ret = code_seq_concat(ret, gen_code_expr(cond->data.bin_cond.rightexp));

	switch (cond->data.bin_cond.relop)
	{
	case (eqop):
		return code_seq_add_to_end(ret, code_eql());
	case (neqop):
		return code_seq_add_to_end(ret, code_neq());
	case (ltop):
		return code_seq_add_to_end(ret, code_lss());
	case (leqop):
		return code_seq_add_to_end(ret, code_leq());
	case (gtop):
		return code_seq_add_to_end(ret, code_gtr());
	case (geqop):
		return code_seq_add_to_end(ret, code_geq());
	default:
		bail_with_error("Call to code_gen_stmt with an AST that is not a binary condition!");
		return code_seq_empty();
	}
}

// generate code for the expresion
code_seq gen_code_expr(AST *exp)
{
	switch (exp->type_tag)
	{
	case (ident_ast):
		return gen_code_ident_expr(exp);
	case (number_ast):
		return gen_code_number_expr(exp);
	case (bin_expr_ast):
		return gen_code_bin_expr(exp);
	default:
		bail_with_error("Call to gen_code_expr with an AST that is not an expression!");
		return code_seq_empty();
	}
}

// generate code for the expression (exp)
code_seq gen_code_bin_expr(AST *exp)
{
	code_seq ret = gen_code_expr(exp->data.bin_expr.leftexp);
	ret = code_seq_concat(ret, gen_code_expr(exp->data.bin_expr.rightexp));

	switch (exp->data.bin_expr.arith_op)
	{
	case (addop):
		return code_seq_add_to_end(ret, code_add());
	case (subop):
		return code_seq_add_to_end(ret, code_sub());
	case (multop):
		return code_seq_add_to_end(ret, code_mul());
	case (divop):
		return code_seq_add_to_end(ret, code_div());
	default:
		bail_with_error("Call to gen_code_bin_expr with an AST that is not a binary expression!");
		return code_seq_empty();
	}
}

// generate code for the ident expression (ident)
code_seq gen_code_ident_expr(AST *ident)
{

	code_seq ret = code_compute_fp(ident->data.ident.idu->levelsOutward);
	int offset = ident->data.ident.idu->attrs->loc_offset;
	ret = code_seq_add_to_end(ret, code_lod(offset));	

	return ret;
}

// generate code for the number expression (num)
code_seq gen_code_number_expr(AST *num)
{
	// int number = num->data.number.value;
	// id_use *numId = num->data.ident.idu;
	// label *numLabel = num->data.proc_decl.lab;
	//  gen_code needs code_seq next, instruction instr, and label *lab
	//  missing instruction instr
	//  instr needs int op, and int m

	return code_lit(num->data.number.value);
}
