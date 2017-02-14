#include "trap.h"

/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
	//first get mth and nth byte, then make mth and nth byte in x equal 0,
	//then put nth byte in mth, and mth in nth.
	int n8=n<<3,m8=m<<3;
	int byte_m=(x>>m8)&0xff,byte_n=(x>>n8)&0xff;	//get two byte
	int x1=0xff<<m8,x2=0xff<<n8;
	x=x&(~x1),x=x&(~x2);		//make nth and mth byte in x equal 0.
	x=x|(byte_m<<n8),x=x|(byte_n<<m8);
    return x;
}

int main()
{
	int x=byteSwap(0x12345678,1,3);

	nemu_assert(x==0x56341278);

	HIT_GOOD_TRAP;

	return 0;
}
