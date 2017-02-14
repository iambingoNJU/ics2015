#include "trap.h"

int sum(char ch,short x,int y)
{
	return ch+x+y;
}

int main()
{
	char a='a';
	short sx=3;
	int y=7;
	int re=sum(a,sx,y);

	nemu_assert(re==107);

	HIT_GOOD_TRAP;

	return 0;
}
