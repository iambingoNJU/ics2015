#include "common.h"
#include "cpu/reg.h"
#include "memory/TLB.h"
#include "memory/memory.h"

typedef union paging_lnaddr {
	uint32_t val;
	struct {
		uint32_t offset	:	12,
				 page	:	10,
				 dir	:	10;
	};
} paging_lnaddr;

uint32_t page_translate2(lnaddr_t addr)
{
	paging_lnaddr lnaddr;
	uint32_t page_dir_addr, page_addr;
	PDE pde;
	PTE pte;

	lnaddr.val = addr;
	//printf("VP: %.5x-->", get_page_number(addr));

	page_dir_addr = (uint32_t)cpu.cr3.page_directory_base << 12;
	pde.val = hwaddr_read(page_dir_addr + lnaddr.dir * 4, 4);
	Assert(pde.present == 1, "Page Dir Miss! At lnaddr == 0x%x.\n", addr);

	page_addr = pde.page_frame << 12;
	pte.val = hwaddr_read(page_addr + lnaddr.page * 4, 4);
	Assert(pte.present == 1, "Page Miss! At lnaddr == 0x%x.\n", addr);
	//printf("PP: %.5x\n", pte.page_frame);

	return pte.page_frame;
}


void TLB_init()
{
	int i;
	for(i=0; i<TLB_SET_SIZE; i++)
		TLB[i].valid = 0;
}

uint32_t TLB_translate(lnaddr_t addr)
{
	uint32_t vp = get_page_number(addr);

	int i;
	for(i=0; i<TLB_SET_SIZE; i++)
		if(TLB[i].valid && (TLB[i].virtual_page == vp))	//Hit
		{
			//printf("TLB HIT!\n");
			break;
		}

	if(i == TLB_SET_SIZE)	//Miss
	{
		//printf("TLB MISS! At lnaddr == 0x%x.\n", addr);
		/* Find an empty position. */
		for(i=0; i<TLB_SET_SIZE; i++)
			if(!TLB[i].valid)	break;

		/* No empty position. Replace one randomly. */
		if(i == TLB_SET_SIZE)
			i = rand() % TLB_SET_SIZE;
		
		/* Fill TLB. */
		TLB[i].valid = 1;
		TLB[i].virtual_page = vp;
		TLB[i].physical_page = page_translate2(addr);	
	}

	return (TLB[i].physical_page << 12) + (addr & 0xfff);
}
