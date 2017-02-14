#include "trap.h"
#include "FLOAT.h"

int main()
{
	int x=-2;
	FLOAT fx=int2F(x);
	nemu_assert(F_mul_F(fx,fx)==int2F(4));

	HIT_GOOD_TRAP;

	return 0;
};
