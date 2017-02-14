#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute () {
	uint8_t opcode=instr_fetch(cpu.eip, 1);
	//printf("opcode: %x\n",opcode);
	if((opcode==0xeb)||(opcode==0xe9))	//jmp rel8/16/32
	{
		cpu.eip+=op_src->val;
		print_asm(str(instr) " %x", cpu.eip + 1 + op_src->size);
	}
	else if(opcode==0xff)	//jmp r/m16,r/m32
	{
		cpu.eip=op_src->val -  1 - concat(decode_rm_,SUFFIX)(cpu.eip+1); 
		print_asm_template1();
	}
	else
	{
		printf("You read the opcode wrong!\n");
	}
}

make_instr_helper(si)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
