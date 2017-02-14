#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute () {
	REG(op_dest->reg)=op_src->addr;
	
	print_asm_template2();	
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
