#include "common.h"
#include "cpu/reg.h"
#include "memory/TLB.h"

//uint32_t dram_read(hwaddr_t, size_t);
//void dram_write(hwaddr_t, size_t, uint32_t);

uint32_t cache1_read(swaddr_t,size_t);
void cache1_write(swaddr_t,size_t,uint32_t);

int is_mmio(hwaddr_t);
uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

/*
typedef union paging_lnaddr {
	uint32_t val;
	struct {
		uint32_t offset	:	12,
				 page	:	10,
				 dir	:	10;
	};
} paging_lnaddr;
*/

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	//uint32_t d1=dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	//uint32_t d2= cache1_read(addr,len) & (~0u >> ((4-len) << 3));
	//uint32_t d=d1^d2;
	//if(d)
	//	printf("len: %d  addr: 0x%x \n dram_read: 0x%x  cache1_read: 0x%x\n\tAT eip= 0x%x\n\n",len,addr,d1,d2,cpu.eip);
	int m = is_mmio(addr);
	if(m == -1)
		return cache1_read(addr,len) & (~0u >> ((4-len) << 3));
	else
		return mmio_read(addr, len, m);
	
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
	//uint32_t d1=dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	int m = is_mmio(addr);
	if(m == -1)
		cache1_write(addr,len,data);
	else
		mmio_write(addr, len, data, m);
	//uint32_t d2= dram_read(addr,len) & (~0u >> ((4-len) << 3));
	//if(d1^d2)
	//	printf("len: %d  dram_write: %x\n cache1_write: %x \n",len,d1,d2);
}

/*
hwaddr_t page_translate(lnaddr_t addr)
{
	paging_lnaddr lnaddr;
	uint32_t page_dir_addr, page_addr;
	PDE pde;
	PTE pte;

	lnaddr.val = addr;

	//printf("Get here.\n");
	page_dir_addr = (uint32_t)cpu.cr3.page_directory_base << 12;
	pde.val = hwaddr_read(page_dir_addr + lnaddr.dir * 4, 4);
	Assert(pde.present == 1, "Page Dir Miss! At lnaddr == 0x%x.\n", addr);

	page_addr = pde.page_frame << 12;
	pte.val = hwaddr_read(page_addr + lnaddr.page * 4, 4);
	Assert(pte.present == 1, "Page Miss! At lnaddr == 0x%x.\n", addr);

	return (pte.page_frame << 12) + lnaddr.offset;
}
*/

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	if(cpu.cr0.protect_enable && cpu.cr0.paging)
	{
		//printf("Before page translate: %x\n", addr);
		if((addr & 0xfff) + len > PAGE_SIZE)
		{
			//printf("Page read cross boundary!\n");
			uint32_t ret, temp1, temp2;
			size_t cur = PAGE_SIZE - (addr & 0xfff),
				   left = len - cur;
			temp1 = lnaddr_read(addr, cur);
			memcpy(&ret, &temp1, cur);
			temp2 = lnaddr_read(addr + cur, left);
			memcpy((char*)(&ret) + cur, &temp2, left);
			return ret;
			//Assert(0, "Page read cross boundary!");
		}
		else 
			addr = TLB_translate(addr);
		//printf("After page translate: %x\n", addr);
	}

	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	if(cpu.cr0.protect_enable && cpu.cr0.paging)
	{
		if((addr & 0xfff) + len > PAGE_SIZE)
		{
			//printf("Page write cross boundary!\n");
			size_t cur = PAGE_SIZE - (addr & 0xfff),
				   left = len - cur;
			lnaddr_write(addr, cur, data);
			lnaddr_write(addr + cur, left, data >> (8*cur));
			return;
			//Assert(0, "Page write cross boundary!");
		}
		else 
			addr = TLB_translate(addr);
	}

	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if(cpu.cr0.protect_enable == 1)
	{
		//Remember to check the limit.
		addr = addr + cpu.sreg[sreg].base;
	}
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if(cpu.cr0.protect_enable == 1)
	{
		//Remember to check the limit.
		addr = addr + cpu.sreg[sreg].base;
	}
	lnaddr_write(addr, len, data);
}

