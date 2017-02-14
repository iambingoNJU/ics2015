#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	DATA_TYPE t1=result ^ (result>>4);
	DATA_TYPE t2=t1 ^ (t1>>2);
	DATA_TYPE t3=t2 ^ (t2>>1);
	cpu.PF=!(t3 & 1);
	cpu.AF=((op_src->val & 0xf) == 0);
	cpu.ZF=(result==0);
	cpu.SF=((DATA_TYPE_S)result < 0);
	t3=0;
	cpu.OF=(op_src->val==(t3-1));
	
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
