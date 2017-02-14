#include "cpu/exec/helper.h"

make_helper(movs2m_b) {
	uint8_t val=swaddr_read(cpu.esi, 1, R_DS);
	swaddr_write(cpu.edi, 1, val, R_ES);
	if(cpu.DF==0)
	{
		cpu.edi++;
		cpu.esi++;
	}
	else
	{
		cpu.edi--;
		cpu.esi--;
	}

	print_asm("movs2mb");

	return 1;
}

make_helper(movs2m_w) {
	uint16_t val=swaddr_read(cpu.esi, 2, R_DS);
	swaddr_write(cpu.edi, 2, val, R_ES);
	if(cpu.DF==0)
	{
		cpu.edi+=2;
		cpu.esi+=2;
	}
	else
	{
		cpu.edi-=2;
		cpu.esi-=2;
	}


	print_asm("movs2mw");

	return 1;
}

make_helper(movs2m_l) {
	uint32_t val=swaddr_read(cpu.esi, 4, R_DS);
	swaddr_write(cpu.edi, 4, val, R_ES);
	if(cpu.DF==0)
	{
		cpu.edi+=4;
		cpu.esi+=4;
	}
	else
	{
		cpu.edi-=4;
		cpu.esi-=4;
	}


	print_asm("movs2ml");

	return 1;
}

make_helper_v(movs2m)
