#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute () {
	uint8_t opcode=swaddr_read(cpu.eip+1, 1, R_CS);
	//printf("opcode: %x \n",opcode);
	if(opcode==0xbe)
	{
		int shift=DATA_BYTE*8-8;
		DATA_TYPE_S val=(((((DATA_TYPE_S)op_src->val)&0xff)<<shift)>>shift);
		OPERAND_W(op_dest, val);
	}
	else if(opcode==0xbf)
	{
		DATA_TYPE_S val=(((((int32_t)op_src->val)&0xffff)<<16)>>16);
		OPERAND_W(op_dest, val);
	}
	else
	{
		printf("You read the wrong opcode!\n");
	}
	print_asm_template2();	
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
