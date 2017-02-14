#include "cpu/exec/template-start.h"

static int concat(leave_,SUFFIX)(swaddr_t eip) {
	cpu.esp=cpu.ebp;

	cpu.ebp=MEM_R(cpu.esp);
	cpu.esp+=DATA_BYTE;

	print_asm("leave");

	return 1;
}


#include "cpu/exec/template-end.h"
