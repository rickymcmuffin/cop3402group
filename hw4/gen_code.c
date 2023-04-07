/* $Id: gen_code_stubs.c,v 1.8 2023/03/29 15:42:08 leavens Exp leavens $ */
#include "utilities.h"
#include "gen_code.h"
#include "symtab.h"


code_seq procedureList;

// Initialize the code generator
void gen_code_initialize()
{
	symtab_initialize();

	// Replace the following with your implementation

	// bail_with_error("gen_code_initialize not implemented yet!");
}

// Generate code for the given AST
code_seq gen_code_program(AST *prog)
{
	return gen_code_block(prog);
}

// generate code for blk
code_seq gen_code_block(AST *blk)
{
	code_seq constDecls = gen_code_constDecs(blk->data.program.cds);
	code_seq varDecls = gen_code_varDecls(blk->data.program.vds);
	// gen_code_procDecls(blk->data.program.pds); // <------------------------------------ add this back eventually

	return code_seq_concat(constDecls, varDecls);
}

// generate code for the declarations in cds
code_seq gen_code_constDecls(AST_list cds)
{
	AST *curr = cds;
	code_seq constDecls;

	// loops through the constant decls
	while (curr != NULL)
	{
		// adds a new constDecl to the end of the code seq
		constDecls = code_seq_add_to_end(constDecls, gen_code_constDecl(curr));
		curr = curr->next;
	}

	return constDecls;
}

// generate code for the const declaration cd
code_seq gen_code_constDecl(AST *cd)
{
	code *newConst = code_lit(cd->data.const_decl.num_val); // generates code
	newConst->lab = scope_lookup(/*gotta figue out how to do parameters*/)->lab; // assigns label
	
	return newConst;
}

// Not checked
// generate code for the declarations in vds
code_seq gen_code_varDecls(AST_list vds)
{
	// iterates through linked list of vds calling gen_code_varDecl on each var AST
	AST_list varDecls = vds;
	code_seq varCodeSeq;
	while (!ast_list_empty_list(vds))
	{
		code_seq_add_to_end(varCodeSeq, gen_code_varDecl(ast_list_singleton(varDecls)));
		varDecls = ast_list_rest(vds);
	}
	
	return varCodeSeq;
}

// generate code for the var declaration vd
code_seq gen_code_varDecl(AST *vd)
{
	// cant use gen_code_ident_expr
	// make it here using code.c functions
	id_use *varId = vd->data.ident.idu;
	
	// Replace the following with your implementation
	bail_with_error("gen_code_varDecl not implemented yet!");
	return code_seq_empty();
}

// generate code for the declarations in pds
void gen_code_procDecls(AST_list pds)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_procDecls not implemented yet!");
}

// generate code for the procedure declaration pd
void gen_code_procDecl(AST *pd)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_procDecl not implemented yet!");
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
		bail_with_error("Call to code_gen_stmt with an AST that is not a statement!");
		return code_seq_empty();
	}
}

// generate code for the statement
code_seq gen_code_assignStmt(AST *stmt)
{
	AST *astIdent = stmt->data.assign_stmt.ident;
	code_seq genIdent = gen_code_ident_expr(astIdent);
	
	id_use *idu = astIdent->data.ident.idu;
	
	

	return code_seq_add_to_end(genIdent, code_sto(idu->attrs->loc_offset));
}

// generate code for the statement
code_seq gen_code_callStmt(AST *stmt)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_callStmt not implemented yet!");
	return code_seq_empty();
}

// generate code for the statement
code_seq gen_code_beginStmt(AST *stmt)
{
	code_seq ret = code_seq_empty();
	AST_list stmts = stmt->data.begin_stmt.stmts;
	while(!ast_list_is_empty(stmts)){
		ret = gen_code_concat(ret, gen_code_stmt(ast_list_first(stmts)));
		stmts = ast_list_rest(stmts);
	}
	return ret;
}

// generate code for the statement
code_seq gen_code_ifStmt(AST *stmt)
{
	//not done
	code_seq cond = code_seq_cond(stmt->data.if_stmt.cond);
	code_seq jpc = code_seq_singleton(code_jpc(2));
	code_seq s1 = code_seq_stmt(stmt->data.if_stmt.thenstmt);	
	code_seq s2 = code_seq_stmt(stmt->data.if_stmt.elsestmt);	
	return code_seq_empty();
}

// generate code for the statement
code_seq gen_code_whileStmt(AST *stmt)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_whileStmt not implemented yet!");
	return code_seq_empty();
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
	// Replace the following with your implementation
	bail_with_error("gen_code_cond not implemented yet!");
	return code_seq_empty();
}

// generate code for the condition
code_seq gen_code_odd_cond(AST *cond)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_odd_cond not implemented yet!");
	return code_seq_empty();
}

// generate code for the condition
code_seq gen_code_bin_cond(AST *cond)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_bin_cond not implemented yet!");
	return code_seq_empty();
}

// generate code for the expresion
code_seq gen_code_expr(AST *exp)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_expr not implemented yet!");
	return code_seq_empty();
}

// generate code for the expression (exp)
code_seq gen_code_bin_expr(AST *exp)
{
	// Replace the following with your implementation
	bail_with_error("gen_code_bin_expr not implemented yet!");
	return code_seq_empty();
}

// generate code for the ident expression (ident)
code_seq gen_code_ident_expr(AST *ident)
{
	
	// Replace the following with your implementation
	bail_with_error("gen_code_ident_expr not implemented yet!");

	id_use *identId = symtab_lookup(ident->data.const_decl.name);

	return code_seq_empty();
}

// generate code for the number expression (num)
code_seq gen_code_number_expr(AST *num)
{
	int number = num->data.number.value;
	id_use *numId = num->data.ident.idu;
	label *numLabel = num->data.proc_decl.lab;
	// gen_code needs code_seq next, instruction instr, and label *lab
	// missing instruction instr
	// instr needs int op, and int m
	
	// Replace the following with your implementation
	bail_with_error("gen_code_number_expr not implemented yet!");
	return code_seq_empty();
}
