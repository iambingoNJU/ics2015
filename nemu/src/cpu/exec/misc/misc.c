#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
void raise_intr(uint8_t);
uint32_t pio_read(ioaddr_t, size_t);
void pio_write(ioaddr_t, size_t, uint32_t);

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(int_n) {
	uint8_t number = instr_fetch(eip + 1, 1);

	//Save old EFLAGS, CS, EIP
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.EFLAGS, R_SS);
	//printf("Saved EFLAGS: 0x%.8x\n", cpu.EFLAGS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 2, cpu.CS.val, R_SS);

	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, eip + 2, R_SS);
	//printf("Saved eip : 0x%x\n", eip + 2);

	raise_intr(number);

	cpu.TF = 0;
	cpu.NT = 0;

	print_asm("int 0x%.2x", number);
	return 2;
}

make_helper(iret) {
	uint32_t temp1, temp2, gdt_addr;
	SegDesc sd;

	cpu.eip = swaddr_read(cpu.esp, 4, R_SS) - 1;		
	cpu.esp += 4;
	//printf("Returned eip : 0x%x\n", cpu.eip + 1);

	cpu.CS.val = swaddr_read(cpu.esp, 2, R_SS);
	cpu.esp += 4;

	/* Load CS cache. */
	gdt_addr = cpu.GDTR.base + cpu.CS.INDEX * 8;
	temp1 = lnaddr_read(gdt_addr, 4);
	temp2 = lnaddr_read(gdt_addr + 4, 4);
	memcpy(&sd, &temp1, 4);
	memcpy((uint8_t*)(&sd) + 4, &temp2, 4);
	Assert(sd.present == 1, "Segment miss in iret!");
	cpu.CS.base = (sd.base_31_24 << 24) + (sd.base_23_16 << 16) + sd.base_15_0;
	cpu.CS.limit = (sd.limit_19_16 << 16) + sd.limit_15_0;

	cpu.EFLAGS = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	//printf("Returned EFLAGS: 0x%.8x\n", cpu.EFLAGS);

	print_asm("iret");
	return 1;
}

make_helper(cli) {
	cpu.IF = 0;

	print_asm("cli");
	return 1;
}

make_helper(sti) {
	cpu.IF = 1;

	print_asm("sti");
	return 1;
}

make_helper(pusha_l) {
	uint32_t old_esp = cpu.esp;
	cpu.esp -= 32;

	swaddr_write(cpu.esp + 28, 4, cpu.eax, R_SS);
	swaddr_write(cpu.esp + 24, 4, cpu.ecx, R_SS);
	swaddr_write(cpu.esp + 20, 4, cpu.edx, R_SS);
	swaddr_write(cpu.esp + 16, 4, cpu.ebx, R_SS);
	swaddr_write(cpu.esp + 12, 4, old_esp, R_SS);
	swaddr_write(cpu.esp + 8,  4, cpu.ebp, R_SS);
	swaddr_write(cpu.esp + 4,  4, cpu.esi, R_SS);
	swaddr_write(cpu.esp,      4, cpu.edi, R_SS);

	print_asm("pushal");
	return 1;
}

make_helper(popa_l) {
	cpu.edi = swaddr_read(cpu.esp,     4, R_SS);
	cpu.esi = swaddr_read(cpu.esp + 4 , 4, R_SS);
	cpu.ebp = swaddr_read(cpu.esp + 8 , 4, R_SS);
	cpu.ebx = swaddr_read(cpu.esp + 16, 4, R_SS);
	cpu.edx = swaddr_read(cpu.esp + 20, 4, R_SS);
	cpu.ecx = swaddr_read(cpu.esp + 24, 4, R_SS);
	cpu.eax = swaddr_read(cpu.esp + 28, 4, R_SS);

	cpu.esp += 32;

	print_asm("popal");
	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(lgidt_w) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	if(m.reg == 2)
	{
		cpu.GDTR.limit = swaddr_read(op_src->addr, 2, R_CS);
		cpu.GDTR.base = swaddr_read(op_src->addr + 2, 4, R_CS) & 0xffffff;

		print_asm("lgdtw %s", op_src->str);
	}
	else if(m.reg == 3)
	{
		cpu.IDTR.limit = swaddr_read(op_src->addr, 2, R_CS);
		cpu.IDTR.base = swaddr_read(op_src->addr + 2, 4, R_CS) & 0xffffff;

		print_asm("lidtw %s", op_src->str);
	}
	else
		Assert(0, "lgidt undefined instruction.");

	return 1 + len;
}

make_helper(lgidt_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	if(m.reg == 2)
	{
		cpu.GDTR.limit = swaddr_read(op_src->addr, 2, R_CS);
		cpu.GDTR.base = swaddr_read(op_src->addr + 2, 4, R_CS);

		print_asm("lgdtl %s", op_src->str);
	}
	else if(m.reg == 3)
	{
		cpu.IDTR.limit = swaddr_read(op_src->addr, 2, R_CS);
		cpu.IDTR.base = swaddr_read(op_src->addr + 2, 4, R_CS);

		print_asm("lidtl %s", op_src->str);
	}
	else
		Assert(0, "lgidt undefined instruction.");

	return 1 + len;
}

make_helper(ct_w) {
	cpu.gpr[R_EDX]._16=(((int16_t)cpu.gpr[R_EAX]._16)>>15);

	print_asm("cwd");
	return 1;
}

make_helper(ct_l) {
	cpu.edx=(((int32_t)cpu.eax)>>31);

	print_asm("cdq");
	return 1;
}

make_helper(mov_cr2r) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.reg == 0)
	{
		cpu.gpr[m.R_M]._32 = cpu.cr0.val;
		print_asm("movl %%cr0,%%%s", regsl[m.R_M]);
	}
	else if( m.reg == 3)
	{
		cpu.gpr[m.R_M]._32 = cpu.cr3.val;
		print_asm("movl %%cr3,%%%s", regsl[m.R_M]);
	}
	return 2;
}

make_helper(mov_r2cr) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.reg == 0)
	{
		cpu.cr0.val = cpu.gpr[m.R_M]._32;
		print_asm("movl %%%s,%%cr0", regsl[m.R_M]);
	}
	else if( m.reg == 3)
	{
		cpu.cr3.val = cpu.gpr[m.R_M]._32;
		print_asm("movl %%%s,%%cr3", regsl[m.R_M]);
	}
	return 2;
}

make_helper(cld) {
	cpu.DF=0;

	print_asm("cld");
	return 1;
}

make_helper(std) {
	cpu.DF=1;

	print_asm("std");
	return 1;
}


make_helper(ljmp_l) {
	cpu.eip = instr_fetch(eip + 1, 4) - 7;
	cpu.CS.val = instr_fetch(eip + 5, 2);
	
	print_asm("ljmp %x:%x", cpu.CS.val, cpu.eip + 7);	
	return 7;
}

make_helper(mov_seg_w) {
	ModR_M m;
	SegDesc sd;
	uint32_t temp1,temp2;

	m.val=instr_fetch(eip + 1, 1);
	cpu.sreg[m.reg].val = cpu.gpr[m.R_M]._16;	

	/* Copy Segment descriptor into cache(invisible part). */
	temp1 = lnaddr_read(cpu.GDTR.base + cpu.sreg[m.reg].INDEX * 8, 4);
	temp2 = lnaddr_read(cpu.GDTR.base + cpu.sreg[m.reg].INDEX * 8 + 4, 4);
	memcpy(&sd, &temp1, 4);
	memcpy((char*)(&sd) + 4, &temp2, 4);

	/* Check the present bit. */
	Assert(sd.present==1, "Segment Miss!\n");

	cpu.sreg[m.reg].base = (sd.base_31_24 << 24) + (sd.base_23_16 << 16) + sd.base_15_0;
	cpu.sreg[m.reg].limit = (sd.limit_19_16 << 16) + sd.limit_15_0;

	print_asm("movw %%%s,%%%s",regsw[m.R_M],sregw[m.reg]);
	return 2;
}

make_helper(in_i2a_b) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	cpu.gpr[R_EAX]._8[0] = pio_read(imm, 1);
	
	print_asm("inb $%x,%%al", imm);
	return 2;
}

make_helper(in_i2a_w) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	cpu.gpr[R_EAX]._16 = pio_read(imm, 2);

	print_asm("inw $%x,%%ax", imm);
	return 2;
}

make_helper(in_i2a_l) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	cpu.gpr[R_EAX]._32 = pio_read(imm, 4);

	print_asm("inl $%x,%%eax", imm);
	return 2;
}

make_helper(in_d2a_b) {
	cpu.gpr[R_EAX]._8[0] = pio_read(cpu.gpr[R_EDX]._16, 1);

	print_asm("inb (%%dx),%%al");
	return 1;
}

make_helper(in_d2a_w) {
	cpu.gpr[R_EAX]._16 = pio_read(cpu.gpr[R_EDX]._16, 2);

	print_asm("inw (%%dx),%%ax");
	return 1;
}

make_helper(in_d2a_l) {
	cpu.gpr[R_EAX]._32 = pio_read(cpu.gpr[R_EDX]._16, 4);

	print_asm("inl (%%dx),%%eax");
	return 1;
}

make_helper(out_a2i_b) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	pio_write(imm, 1, cpu.gpr[R_EAX]._8[0]);

	print_asm("outb %%al,$%x", imm);
	return 2;
}

make_helper(out_a2i_w) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	pio_write(imm, 2, cpu.gpr[R_EAX]._16);

	print_asm("outb %%ax,$%x", imm);
	return 2;
}

make_helper(out_a2i_l) {
	uint8_t imm = instr_fetch(eip + 1, 1);
	pio_write(imm, 4, cpu.gpr[R_EAX]._32);

	print_asm("outb %%eax,$%x", imm);
	return 2;
}

make_helper(out_a2d_b) {
	pio_write(cpu.gpr[R_EDX]._16, 1, cpu.gpr[R_EAX]._8[0]);

	print_asm("outa %%al,(%%dx)");
	return 1;
}

make_helper(out_a2d_w) {
	pio_write(cpu.gpr[R_EDX]._16, 2, cpu.gpr[R_EAX]._16);

	print_asm("outa %%ax,(%%dx)");
	return 1;
}

make_helper(out_a2d_l) {
	pio_write(cpu.gpr[R_EDX]._16, 4, cpu.gpr[R_EAX]._32);

	print_asm("outa %%eax,(%%dx)");
	return 1;
}

make_helper(hlt) {
	Assert(cpu.IF == 1, "You can't fall into halt when IF is 0.");
	printf("In hlt ");
	while(!cpu.INTR) printf(".");
	printf("\n");

	print_asm("hlt");
	return 1;
}

make_helper_v(ct)
make_helper_v(lgidt)
make_helper_v(in_i2a)
make_helper_v(in_d2a)
make_helper_v(out_a2i)
make_helper_v(out_a2d);

