#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE == 2
static void do_execute () {
	uint32_t ip=swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp+=4;
	
	cpu.esp+=op_src->val;

	cpu.eip=ip-1-op_src->size;

	print_asm("reti");

}

make_instr_helper(i)
#endif

make_helper(concat(ret_,SUFFIX)) {
	DATA_TYPE ip=MEM_R(cpu.esp);
	cpu.esp+=DATA_BYTE;

	cpu.eip=ip-1;

	print_asm("ret");

	return 1;
}

#include "cpu/exec/template-end.h"
