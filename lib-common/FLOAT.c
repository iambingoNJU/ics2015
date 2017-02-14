#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	//return ((a>>4)*(b>>4))>>8;
	//long long r=((long long)a * b)>>16;
	//return (FLOAT)(r & 0xffffffff);	//if I use this way, shrd instruction is not surported.

	union {
		long long l;
		struct {
			unsigned short twobyte1,twobyte2,twobyte3,twobyte4;
		};
	}temp;

	temp.l=(long long)a*b;
	return (unsigned int)(temp.twobyte2) + (((unsigned int)temp.twobyte3)<<16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	return (a<<13)/(b>>3);
	//This is other pepele's help.
/*	unsigned sign=(a & 0x80000000) ^ (b & 0x80000000);
	unsigned ua=a & 0x7fffffff,
			 ub=b & 0x7fffffff;
	int result=((ua/ub)<<16);
	unsigned remain=ua%ub;
	int i=0;
	for(;i<16;i++)
	{
		remain<<=1;
		if(sign==0)
			result+=(remain/ub)<<(15-i);
		else
			result-=(remain/ub)<<(15-i);
		remain%=ub;
	}
	return result | sign;
*/	
}

FLOAT f2F(float a) {
	union fi {
		float f;
		int i;
	}temp;
	temp.f=a;

	int exp=((temp.i & 0x7f800000)>>23) - 127;
	int m=(temp.i & 0x7fffff) + 0x800000;
	int sign=0x80000000 & temp.i;
	//Assert(exp<=14, "float to FLOAT overflow!");

	FLOAT result;
	if(exp>7)
		result=m<<(exp-7);	
	else
		result=m>>(7-exp);

	if(sign==0)
		return result;
	else
		return -result;
}

FLOAT Fabs(FLOAT a) {
	return (a>=0) ? a : (-a);
}


FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

