#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/cpu.h"
#include "../../lib-common/x86-inc/mmu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef union {
	struct {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

		struct {
			union {
				uint16_t val;
				struct {
					uint16_t RPL	:	2,
							 TI		:	1,
							 INDEX	:	13;
				};
			};
			uint32_t base, limit;
		} sreg[4];
	};

	/* Do NOT change the order of the GPRs' definitions. */

	//uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

	//swaddr_t eip;
	struct {
		uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;

		struct {
			union {
				uint16_t val;
				struct {
					uint16_t RPL	:	2,
							 TI		:	1,
							 INDEX	:	13;
				};
			};
			uint32_t base, limit;
		} ES, CS, SS, DS;

		swaddr_t eip;

		struct {
			uint16_t limit;
			uint32_t base;
		} GDTR, IDTR;

		CR0 cr0;
		CR3 cr3;

		bool INTR;

		union {
			uint32_t EFLAGS;
			struct {
				uint32_t CF:1,
						 reserved1:1,
						 PF:1,
						 reserved2:1,
						 AF:1,
						 reserved3:1,
						 ZF:1,
						 SF:1,
						 TF:1,
						 IF:1,
						 DF:1,
						 OF:1,
						 OL:1,
						 IP:1,
						 NT:1,
						 reserved4:1,
						 RF:1,
						 VM:1,
						 reserved5:14;
			};
		};
	};

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sregw[];

#endif
