#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
void serial_printc(char);
uint32_t swaddr_read(uint32_t, size_t, uint8_t);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

static void sys_write(TrapFrame *tf) {
	if(tf->ebx==1 || tf->ebx==2)
	{
		uint32_t len = tf->edx, buf = tf->ecx;
		asm volatile (".byte 0xd6" : : "a"(2), "c"(buf), "d"(len));
		/*uint32_t i;
		assert(len != 0);
		for(i=0; i<len; i++)
			serial_printc((char)(swaddr_read(buf+i, 1, 3)));
		serial_printc('\n');
		*/tf->eax = len;
	}
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: sys_brk(tf); break;

		/* TODO: Add more system calls. */
		case SYS_write: sys_write(tf); break;

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

