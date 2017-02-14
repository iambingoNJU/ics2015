#ifndef __TLB_H__
#define __TLB_H__

/* TLB is fully associative. */
#define TLB_SET_SIZE 64

#define get_page_number(addr) ((uint32_t)addr >> 12)

typedef struct {
	uint32_t valid			:	1,
			 virtual_page	:	20,
			 physical_page	:	20;
} TLB_line;

/* The simulated TLB. */
TLB_line TLB[TLB_SET_SIZE];

void TLB_init();
uint32_t TLB_translate(lnaddr_t addr);

#endif
