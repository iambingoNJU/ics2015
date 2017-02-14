#include "cpu/exec/template-start.h"

static void do_execute () {
//	printf("eip: %x  flag: %d\n", cpu.eip,flag);
	if(flag)
		cpu.eip+=op_src->simm;

	//printf("simm: %d\n", op_src->simm);
	print_asm(str(instr) " %x", cpu.eip + 1 + op_src->size);

}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
