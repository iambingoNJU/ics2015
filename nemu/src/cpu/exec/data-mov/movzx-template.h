#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute () {
	uint8_t opcode=swaddr_read(cpu.eip+1, 1, R_CS);
	//printf("val: %x  opcode:%x\n",op_src->val&0xff,opcode);
	if(opcode==0xb6)
		OPERAND_W(op_dest, op_src->val & 0xff);
	else if(opcode==0xb7)
		OPERAND_W(op_dest, op_src->val & 0xffff);
	else
		printf("You read the wrong opcode!\n");

	print_asm_template2();	
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
