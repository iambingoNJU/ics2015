#include "common.h"
#include "memory/cache1.h"
#include "memory/cache2.h"

/* Init the cache1 everytime NEMU restarts. */
void cache1_init()
{
	int i,j;
	for(i=0;i<CACHE1_NR_SET;i++)
		for(j=0;j<CACHE1_SET_SIZE;j++)
			cache1[i][j].valid=0;	
}

/* Read data from cache1. */
uint32_t cache1_read(hwaddr_t addr,size_t size)
{
	unsigned i,j;	
	uint32_t ret;
	cache1_addr temp;
	temp.addr=addr;

	i=temp.row;
	for(j=0;j<CACHE1_SET_SIZE;j++)
		if(cache1[i][j].valid && (cache1[i][j].tag==temp.tag))	//Hit
		{
			with_cache_time += 2;
			no_cache_time += 200;
			//printf("cache1 read HIT!\n");
			break;
		}

	if(j==CACHE1_SET_SIZE)	//Miss! 
	{
		with_cache_time += 200;
		no_cache_time += 200;
		/* Find an empty line. */
		for(j=0;j<CACHE1_SET_SIZE;j++)
			if(!cache1[i][j].valid)	break;

		/* Here needs to replace the old cache1 line. replace randomly. */
		if(j==CACHE1_SET_SIZE)	
			j=rand()%CACHE1_SET_SIZE;

		/* Read data from cache2 to cache1. */
		cache2_read(addr/CACHE1_BLOCK_SIZE*CACHE1_BLOCK_SIZE, CACHE1_BLOCK_SIZE, cache1[i][j].data);
		cache1[i][j].valid=1;
		cache1[i][j].tag=temp.tag;
		//printf("cache1 read MISS! j=%d\n",j);
	}

	if(temp.col+size > CACHE1_BLOCK_SIZE)
	{
		//printf("Cache1 corss block boundary!\n");
		cache2_read(addr,size,(uint8_t*)(&ret));
	}
	else
		memcpy(&ret,cache1[i][j].data+temp.col,size);	//remember to deal with crossing boundary problem.

	return ret;
}

/* Write data to cache1. write through, not allocate. */
void cache1_write(hwaddr_t addr,size_t size,uint32_t data)
{
	unsigned i,j;
	cache1_addr temp;
	temp.addr=addr;

	i=temp.row;
	for(j=0;j<CACHE1_SET_SIZE;j++)
		if(cache1[i][j].valid && (cache1[i][j].tag==temp.tag))	//Hit
		{
			with_cache_time += 2;
			no_cache_time += 200;
			memcpy(cache1[i][j].data+temp.col,&data,size);
			cache2_write(addr, size, data);
			//printf("cache1 write HIT!\n");
			break;
		}

	if(j==CACHE1_SET_SIZE)	//Miss
	{
		with_cache_time += 200;
		no_cache_time += 200;
		cache2_write(addr, size, data);
		//printf("cache1 write MISS!\n");
	}
}

