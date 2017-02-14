#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	//const uint32_t elf_magic = 0xBadC0de;
	//uint32_t *p_magic = (void *)buf;
	//nemu_assert(*p_magic == elf_magic);
	//The following is copied from 'nemu/src/monitor/debug/elf.c'
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load each program segment */
	ph=(Elf32_Phdr*)(buf + elf->e_phoff);
	int num=elf->e_phnum,i=0;
	for(; i<num;i++ ) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			uint32_t hwaddr = mm_malloc(ph->p_vaddr, ph->p_memsz);
			//ide_read((uint8_t*)pa_to_va(hwaddr), ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
			memcpy((uint8_t*)pa_to_va(hwaddr), (uint8_t*)(ELF_OFFSET_IN_DISK+ph->p_offset), ph->p_filesz);

			//strncpy((char*)ph->p_vaddr,(char*)(buf+ph->p_offset),ph->p_filesz);
			//ramdisk_read((uint8_t*)ph->p_vaddr, ELF_OFFSET_IN_DISK+ph->p_offset, ph->p_filesz);
			//memcpy((uint8_t*)ph->p_vaddr, (uint8_t*)(ELF_OFFSET_IN_DISK+ph->p_offset), ph->p_filesz);
			 

			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			//memset((char*)(ph->p_vaddr + ph->p_filesz),0,ph->p_memsz-ph->p_filesz);
			memset((uint8_t*)(pa_to_va(hwaddr) + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);

#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
		ph=ph+1;
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
