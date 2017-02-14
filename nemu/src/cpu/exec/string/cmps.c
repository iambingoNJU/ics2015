#include "cpu/exec/helper.h"

make_helper(cmps2m_b) {
	uint8_t src=swaddr_read(cpu.edi, 1, R_ES),
			dest=swaddr_read(cpu.esi, 1, R_ES);
	uint8_t result=dest-src;
	if(cpu.DF==0)
	{
		cpu.esi++;
		cpu.edi++;
	}
	else
	{
		cpu.esi--;
		cpu.edi--;
	}

	cpu.CF=(dest < src);
	uint8_t t1=result ^ (result>>4);
	uint8_t t2=t1 ^ (t1>>2);
	uint8_t t3=t2 ^ (t2>>1);
	cpu.PF=!(t3 & 1);
	cpu.AF=((dest & 0xf) < (src & 0xf));
	cpu.ZF=(result==0);
	cpu.SF=((int8_t)result < 0);
	cpu.OF=(((int8_t)dest < 0) != ((int8_t)src < 0)) && (((int8_t)dest < 0) != ((int8_t)result < 0));
	
	print_asm("cmps2mb");

	return 1;
}

make_helper(cmps2m_w) {
	uint16_t src=swaddr_read(cpu.edi, 2, R_ES),
			 dest=swaddr_read(cpu.esi, 2, R_ES);
	uint16_t result=dest-src;
	if(cpu.DF==0)
	{
		cpu.esi+=2;
		cpu.edi+=2;
	}
	else
	{
		cpu.esi-=2;
		cpu.edi-=2;
	}

	cpu.CF=(dest < src);
	uint8_t t1=result ^ (result>>4);
	uint8_t t2=t1 ^ (t1>>2);
	uint8_t t3=t2 ^ (t2>>1);
	cpu.PF=!(t3 & 1);
	cpu.AF=((dest & 0xf) < (src & 0xf));
	cpu.ZF=(result==0);
	cpu.SF=((int16_t)result < 0);
	cpu.OF=(((int16_t)dest < 0) != ((int16_t)src < 0)) && (((int16_t)dest < 0) != ((int16_t)result < 0));
	
	print_asm("cmps2mw");

	return 1;
}

make_helper(cmps2m_l) {
	uint32_t src=swaddr_read(cpu.edi, 4, R_ES),
			 dest=swaddr_read(cpu.esi, 4, R_ES);
	uint32_t result=dest-src;
	if(cpu.DF==0)
	{
		cpu.esi+=4;
		cpu.edi+=4;
	}
	else
	{
		cpu.esi-=4;
		cpu.edi-=4;
	}

	cpu.CF=(dest < src);
	uint32_t t1=result ^ (result>>4);
	uint32_t t2=t1 ^ (t1>>2);
	uint32_t t3=t2 ^ (t2>>1);
	cpu.PF=!(t3 & 1);
	cpu.AF=((dest & 0xf) < (src & 0xf));
	cpu.ZF=(result==0);
	cpu.SF=((int32_t)result < 0);
	cpu.OF=(((int32_t)dest < 0) != ((int32_t)src < 0)) && (((int32_t)dest < 0) != ((int32_t)result < 0));
	
	print_asm("cmps2ml");

	return 1;
}

make_helper_v(cmps2m)
