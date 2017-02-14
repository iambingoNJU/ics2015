#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp-=4;
	swaddr_write(cpu.esp, 4, 0, R_SS);
	MEM_W(cpu.esp, cpu.eip + 1 + op_src->size );

	uint8_t opcode=instr_fetch(cpu.eip, 1);
	if(opcode==0xe8)	//call rel16, rel32
	{
		cpu.eip+=op_src->val;
		print_asm(str(instr) " %x" , cpu.eip + 1 + op_src->size);
	}
	else if(opcode==0xff)	//call r/m16, r/m32
	{
		cpu.eip=op_src->val - 1 - concat(decode_rm_,SUFFIX)(cpu.eip+1);	
		print_asm(str(instr) " *%s",op_src->str);
	}
	else
		printf("You read the wrong opcode!\n");

}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
