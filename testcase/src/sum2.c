#include "trap.h"

int sum(int x,int y)
{
	return x+y;
}

int main()
{
	int m=3,n=4;
	int s=sum(m,n);
	nemu_assert(s==7);

	HIT_GOOD_TRAP;

	return 0;
}

