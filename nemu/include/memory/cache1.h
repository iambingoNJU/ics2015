#ifndef __CACHE1_H__
#define __CACHE1_H__

#define CACHE1_SIZE (64*1024)
#define CACHE1_BLOCK_SIZE 64
#define CACHE1_BLOCK_BITS 6
#define CACHE1_SET_SIZE 8
#define CACHE1_NR_SET (CACHE1_SIZE / CACHE1_BLOCK_SIZE / CACHE1_SET_SIZE)
#define CACHE1_SET_BITS 7
#define CACHE1_TAG_BITS (32 - CACHE1_BLOCK_BITS - CACHE1_SET_BITS)

//typedef bool (*MEM_ACCESS_FUNC)(hwaddr_t,size_t,uint32_t);

typedef struct {
	struct {
		uint32_t valid	:	1;
		uint32_t tag	:	CACHE1_TAG_BITS;
	};
	uint8_t data[CACHE1_BLOCK_SIZE];
} cache1_line;

/* The simulated cache1. */
cache1_line cache1[CACHE1_NR_SET][CACHE1_SET_SIZE];

typedef union {
	struct {
		uint32_t col	:	CACHE1_BLOCK_BITS;
		uint32_t row	:	CACHE1_SET_BITS;
		uint32_t tag	:	CACHE1_TAG_BITS;
	};
	uint32_t addr;
} cache1_addr;

void cache1_init();
uint32_t cache1_read(hwaddr_t,size_t);
void cache1_write(hwaddr_t,size_t,uint32_t);

/*
typedef struct {
	cache1_line cache1[CACHE1_NR_SET][CACHE1_SET_SIZE];
	MEM_ACCESS_FUNC read,write;
} Cache;

Cache CPU_cache1;
CPU_cache1.read=cache1_read;
*/

#endif
