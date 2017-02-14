#ifndef __CACHE2_H__
#define __CACHE2_H__

#define CACHE2_SIZE (4*1024*1024)
#define CACHE2_BLOCK_SIZE 64
#define CACHE2_SET_SIZE 16
#define CACHE2_NR_SET (CACHE2_SIZE / CACHE2_BLOCK_SIZE / CACHE2_SET_SIZE)
#define CACHE2_BLOCK_BITS 6
#define CACHE2_SET_BITS 12
#define CACHE2_TAG_BITS (32 - CACHE2_BLOCK_BITS - CACHE2_SET_BITS)

long long with_cache_time, no_cache_time;

typedef struct {
	struct {
		uint32_t dirty	:	1;
		uint32_t valid	:	1;
		uint32_t tag	:	CACHE2_TAG_BITS;
	};
	uint8_t data[CACHE2_BLOCK_SIZE];
} cache2_line;

/* The simulated cache2. */
cache2_line cache2[CACHE2_NR_SET][CACHE2_SET_SIZE];

typedef union {
	struct {
		uint32_t col	:	CACHE2_BLOCK_BITS;
		uint32_t row	:	CACHE2_SET_BITS;
		uint32_t tag	:	CACHE2_TAG_BITS;
	};
	uint32_t addr;
} cache2_addr;

void cache2_init();
void cache2_read(hwaddr_t,size_t,uint8_t*);
void cache2_write(hwaddr_t,size_t,uint32_t);

#endif
