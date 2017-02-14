#include "common.h"
#include "memory/memory.h"
#include "memory/cache2.h"

/* Init the cache2 everytime NEMU restarts. */
void cache2_init()
{
	int i,j;
	for(i=0;i<CACHE2_NR_SET;i++)
		for(j=0;j<CACHE2_SET_SIZE;j++)
		{
			cache2[i][j].valid=0;	
			cache2[i][j].dirty=0;
		}
}

/* Read data from cache2. */
void cache2_read(hwaddr_t addr,size_t size, uint8_t *data)
{
	unsigned i,j;	
	cache2_addr temp;
	temp.addr=addr;

	i=temp.row;
	for(j=0;j<CACHE2_SET_SIZE;j++)
		if(cache2[i][j].valid && (cache2[i][j].tag==temp.tag))	//Hit
		{
			//printf("cache2 read HIT!\n");
			break;
		}

	if(j==CACHE2_SET_SIZE)	//Miss! 
	{
		/* Find an empty line. */
		for(j=0;j<CACHE2_SET_SIZE;j++)
			if(!cache2[i][j].valid)	break;

		/* Here needs to replace the old cache line. replace randomly. */
		if(j==CACHE2_SET_SIZE)	
			j=rand()%CACHE2_SET_SIZE;

		/* First write back, then read data from memory to cache2. */
		if(cache2[i][j].valid && cache2[i][j].dirty)	//write back
		{
			cache2_addr rpa;
			rpa.tag=cache2[i][j].tag;
			rpa.row=i;
			rpa.col=0;
			memcpy(hw_mem+rpa.addr, cache2[i][j].data, CACHE2_BLOCK_SIZE);		
			cache2[i][j].dirty=0;
		}
		memcpy(cache2[i][j].data, hw_mem + addr/CACHE2_BLOCK_SIZE*CACHE2_BLOCK_SIZE, CACHE2_BLOCK_SIZE);
		cache2[i][j].valid=1;
		cache2[i][j].tag=temp.tag;
		//printf("cache2 read MISS! j=%d\n",j);
	}

	//printf("cache2 read size: %d  addr: %x\n",size,addr);
	if(temp.col+size > CACHE2_BLOCK_SIZE)
	{
		//printf("Cache2 cross block boundary!\n");
		uint32_t cur=CACHE2_BLOCK_SIZE - temp.col, left=size - cur;
		memcpy(data,cache2[i][j].data+temp.col,cur);
		cache2_read(addr+cur,left,data+cur);
	}
	else
		memcpy(data,cache2[i][j].data+temp.col,size);	//remember to deal with crossing boundary problem.
}

/* Write data to cache2. */
void cache2_write(hwaddr_t addr,size_t size,uint32_t data)
{
	unsigned i,j;
	cache2_addr temp;
	temp.addr=addr;

	i=temp.row;
	for(j=0;j<CACHE2_SET_SIZE;j++)
		if(cache2[i][j].valid && (cache2[i][j].tag==temp.tag))	//Hit
		{
			//printf("cache2 write HIT!\n");
			break;
		}

	if(j==CACHE2_SET_SIZE)	//Miss: write allocate
	{
		/* Find an empty line. */
		for(j=0;j<CACHE2_SET_SIZE;j++)
			if(!cache2[i][j].valid)	break;

		/* Here needs to replace the old cache2 line. */
		if(j==CACHE2_SET_SIZE)	
			j=rand()%CACHE2_SET_SIZE;

		/* First write back, then read data from memory to cache2. */
		if(cache2[i][j].valid && cache2[i][j].dirty)	//write back
		{
			cache2_addr rpa;
			rpa.tag=cache2[i][j].tag;
			rpa.row=i;
			rpa.col=0;
			memcpy(hw_mem+rpa.addr, cache2[i][j].data, CACHE2_BLOCK_SIZE);		
			cache2[i][j].dirty=0;
		}

		memcpy(cache2[i][j].data, hw_mem + addr/CACHE2_BLOCK_SIZE*CACHE2_BLOCK_SIZE, CACHE2_BLOCK_SIZE);
		cache2[i][j].valid=1;
		cache2[i][j].tag=temp.tag;
		//printf("cache2 write MISS!\n");
	}

	/* Write the cache. */
	if(temp.col+size > CACHE2_BLOCK_SIZE)
	{
		//printf("cache2 write cross block boundary!\n");
		uint32_t cur=CACHE2_BLOCK_SIZE - temp.col, left=size - cur;
		memcpy(cache2[i][j].data+temp.col,&data,cur);
		cache2_write(addr+cur, left, data>>(8*cur));
	}
	else
	{
		memcpy(cache2[i][j].data+temp.col,&data,size);
		cache2[i][j].dirty=1;
	}

}

