#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE result = op_dest->val ^ op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	cpu.CF=0;
	cpu.OF=0;
	cpu.ZF=(result==0);
	cpu.SF=((DATA_TYPE_S)result<0);
	DATA_TYPE t1=(result ^ (result>>4));
	DATA_TYPE t2=(t1 ^ (t1>>2));
	DATA_TYPE t3=(t2 ^ (t2>>1));
	cpu.PF=t3 & 1;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
