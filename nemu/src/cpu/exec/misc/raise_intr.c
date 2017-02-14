#include <setjmp.h>
#include "cpu/exec/helper.h"
extern jmp_buf jbuf;

void raise_intr(uint8_t NO) {
	uint32_t temp1, temp2, idt_addr, gdt_addr;
	GateDesc gd;
	SegDesc sd;

	// Load selector to CS, offset to eip.

	/* Get the gate descriptor. */
	idt_addr = cpu.IDTR.base + NO * 8;
	temp1 = lnaddr_read(idt_addr, 4);
	temp2 = lnaddr_read(idt_addr + 4, 4);
	memcpy(&gd, &temp1, 4);
	memcpy(((uint8_t*)(&gd)) + 4, &temp2, 4);

	/* Check the present bit. */
	Assert(gd.present == 1, "Idt not exsit!");

	/* Check gate descriptor interupt type. */
	Assert(gd.type==0xe || gd.type==0xf, "Undefined exception type!"); 

	cpu.eip = (gd.offset_31_16 << 16) + gd.offset_15_0;

	/* Get the segment descriptor. */
	cpu.CS.val = gd.segment;
	gdt_addr = cpu.GDTR.base + cpu.CS.INDEX * 8;
	temp1 = lnaddr_read(gdt_addr, 4);
	temp2 = lnaddr_read(gdt_addr + 4, 4);
	memcpy(&sd, &temp1, 4);
	memcpy((uint8_t*)(&sd) + 4, &temp2, 4);

	/* Check the present bit. */
	Assert(sd.present == 1, "Segment miss!");

	/* Get the exception procedure addr. */
	cpu.CS.base = (sd.base_31_24 << 24) + (sd.base_23_16 << 16) + sd.base_15_0;
	cpu.CS.limit = (sd.limit_19_16 << 8) + sd.limit_15_0;

	if(gd.type == 0xe)	cpu.IF = 0;

	longjmp(jbuf, 1);
}
