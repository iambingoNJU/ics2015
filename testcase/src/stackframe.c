#include "trap.h"

int add(int n) {
	if(n>0)
		return add(n-1)+n;
	else
		return 0;
}

int main() {
	int x=add(5);

	nemu_assert(x==15);

	HIT_GOOD_TRAP;

	return 0;
}
