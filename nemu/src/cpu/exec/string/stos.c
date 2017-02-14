#include "cpu/exec/helper.h"

make_helper(stos2m_b) {
	swaddr_write(cpu.edi, 1, cpu.gpr[R_EAX]._8[0], R_ES);
	if(cpu.DF==0)
		cpu.edi++;
	else
		cpu.edi--;

	print_asm("stos2mb");

	return 1;
}

make_helper(stos2m_w) {
	swaddr_write(cpu.edi, 2, cpu.gpr[R_EAX]._16, R_ES);
	if(cpu.DF==0)
		cpu.edi+=2;
	else
		cpu.edi-=2;

	print_asm("stos2mw");

	return 1;
}

make_helper(stos2m_l) {
	swaddr_write(cpu.edi, 4, cpu.gpr[R_EAX]._32, R_ES);
	if(cpu.DF==0)
		cpu.edi+=4;
	else
		cpu.edi-=4;

	print_asm("stos2ml");

	return 1;
}

make_helper_v(stos2m)
