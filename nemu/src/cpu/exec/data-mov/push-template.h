#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	//REG(R_ESP)-=DATA_BYTE;
	cpu.esp-=4;	//stack should align 4
	//printf("val: %d\n",op_src->val);	//why when push $8, it becomes 0x7ffff08
	swaddr_write(cpu.esp, 4, 0, R_SS);
	MEM_W(cpu.esp, op_src->val);	//addr should be cpu.esp, not REG(R_ESP)

	print_asm_template1();
	return;
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
make_instr_helper(r)
#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"
