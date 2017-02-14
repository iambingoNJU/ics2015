#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
uint32_t var_addr(char* str);

enum {
	//The value of operator represents their priority. But not brackets.
	REG, VAR, NUM, LSB, RSB, NOTYPE, OR=260, AND=270, EQ=280, NEQ,
   	PLUS=290, SUB, MULT=300, DEVI, NOT=310, DEREF, NEG, POS,

	/* TODO: Add more token types */
		//Done!

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	//Done!
	{"[0-9]+|0[xX][0-9a-fA-F]+|0[0-7]+", NUM},
	{"\\$e?[abcd]x|\\$e?[bs]p|\\$e?[ds]i|\\$[abcd][hl]|\\$eip|INTR", REG},	//remember not to add space randomly! You have made a mistake.
	{"[a-zA-Z][a-zA-Z0-9_]*", VAR},		//I'm not sure whether it's right to the name of variable.

	{" +",	NOTYPE},				// spaces
	{"==", EQ},						// equal
	{"!=", NEQ},
	{"&&", AND},
	{"\\|\\|", OR},
	{"!", NOT},
	{"\\+", PLUS},					// plus
	{"-",  SUB},
	{"\\*",  MULT},
	{"/",  DEVI},
	{"\\(",  LSB},
	{"\\)",  RSB},
	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				//Done!

				switch(rules[i].token_type) {
					case PLUS: 
					case SUB:
					case MULT:
					case DEVI:
					case LSB:
					case RSB:
					case EQ:
					case NEQ:
					case AND:
					case OR:
					case NOT:
						tokens[nr_token].type=rules[i].token_type;
						strcpy(tokens[nr_token].str,"");
						nr_token++;
						break;
					case NOTYPE:
						break;
					case NUM:
					case VAR:
					case REG:
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';	//strncpy may not copy the '\0' char, because you limit the number to be copied.
						nr_token++;
						break;
					default: printf("No such tokens!\n");return false;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

static bool check_parentheses(uint32_t p,uint32_t q)
{
	if((tokens[p].type==LSB)&&(tokens[q].type==RSB))
	{
		uint32_t i=p+1;
		int count=0;
		for(;i<q;i++)
		{
			if(tokens[i].type==LSB)
				count++;
			else if(tokens[i].type==RSB)
				count--;

			if(count<0)	return false;
		}
		Assert(count==0,"Invalid expression! Brakets not match!");
		
		return true;
	}
	return false;
}

static uint32_t divide_position(uint32_t p,uint32_t q)
{
	bool flag=true;
	uint32_t pos=p,i=p;
	for(;i<q;i++)
	{
		//operator in brakets can't be the divide_position, so just ignore it.
		if(tokens[i].type==LSB)
		{
			int cnt=1;
			while(cnt!=0)
			{
				i++;
				if(tokens[i].type==LSB)
					cnt++;
				else if(tokens[i].type==RSB)
					cnt--;
			}
			i++;
		}

		if((i<q)&&(tokens[i].type/10))
		{
			if(flag)
			{
				pos=i;
				flag=false;
			}
			//to find the operator whose priority is the lowest.
			else if(tokens[i].type/10<tokens[pos].type/10)
				pos=i;
		}
	}

	return pos;
}

static uint32_t eval(uint32_t p,uint32_t q)
{
	if(p>q)
	{
		printf("As a programmer, you should check your code carefully!\n");
		assert(0);
	}
	else if(p==q)
	{
		uint32_t result;
		if(tokens[p].type==NUM)
		{
			result=strtol(tokens[p].str,NULL,0);
			return result;
		}
		else if(tokens[p].type==REG)
		{
			if(strcmp(tokens[p].str,"$eax")==0)
				return cpu.eax;
			else if(strcmp(tokens[p].str,"$ebx")==0)
				return cpu.ebx;
			else if(strcmp(tokens[p].str,"$ecx")==0)
				return cpu.ecx;
			else if(strcmp(tokens[p].str,"$edx")==0)
				return cpu.edx;
			else if(strcmp(tokens[p].str,"$esp")==0)
				return cpu.esp;
			else if(strcmp(tokens[p].str,"$ebp")==0)
				return cpu.ebp;
			else if(strcmp(tokens[p].str,"$esi")==0)
				return cpu.esi;
			else if(strcmp(tokens[p].str,"$edi")==0)
				return cpu.edi;
			else if(strcmp(tokens[p].str,"$eip")==0)
				return cpu.eip;

			else if(strcmp(tokens[p].str,"$ax")==0)
				return cpu.gpr[R_EAX]._16;
			else if(strcmp(tokens[p].str,"$bx")==0)
				return cpu.gpr[R_EBX]._16;
			else if(strcmp(tokens[p].str,"$cx")==0)
				return cpu.gpr[R_ECX]._16;
			else if(strcmp(tokens[p].str,"$dx")==0)
				return cpu.gpr[R_EDX]._16;
			else if(strcmp(tokens[p].str,"$sp")==0)
				return cpu.gpr[R_ESP]._16;
			else if(strcmp(tokens[p].str,"$bp")==0)
				return cpu.gpr[R_EBP]._16;
			else if(strcmp(tokens[p].str,"$si")==0)
				return cpu.gpr[R_ESI]._16;
			else if(strcmp(tokens[p].str,"$di")==0)
				return cpu.gpr[R_EDI]._16;

			else if(strcmp(tokens[p].str,"$ah")==0)
				return cpu.gpr[R_EAX]._8[1];
			else if(strcmp(tokens[p].str,"$al")==0)
				return cpu.gpr[R_EAX]._8[0];
			else if(strcmp(tokens[p].str,"$bh")==0)
				return cpu.gpr[R_EBX]._8[1];
			else if(strcmp(tokens[p].str,"$bl")==0)
				return cpu.gpr[R_EBX]._8[0];
			else if(strcmp(tokens[p].str,"$ch")==0)
				return cpu.gpr[R_ECX]._8[1];
			else if(strcmp(tokens[p].str,"$cl")==0)
				return cpu.gpr[R_ECX]._8[0];
			else if(strcmp(tokens[p].str,"$dh")==0)
				return cpu.gpr[R_EDX]._8[1];
			else if(strcmp(tokens[p].str,"$dl")==0)
				return cpu.gpr[R_EDX]._8[0];

			else if(strcmp(tokens[p].str,"INTR")==0)
				return cpu.INTR;

		}
		else if(tokens[p].type==VAR)
		{
			//printf("VAR expression will be implemented after!\n");
			uint32_t result=var_addr(tokens[p].str);
			if(result==0)
				printf("No such global variable '%s'! The following answer is wrong!\n",tokens[p].str);
			return result;
		}
	}
	else
	{
		if(check_parentheses(p,q)==true)
			return eval(p+1,q-1);

		uint32_t op=divide_position(p,q);

		if(p==op)
		{
			uint32_t val=eval(p+1,q);
			switch(tokens[op].type)
			{
				case NOT:   return !val;
				case DEREF:	return swaddr_read(val,4,R_DS);
				case POS:	return val;
				case NEG:	return -val;
				default: printf("Unknown unary operator!\n");
			}
		}

		else
		{
			uint32_t val1=eval(p,op-1);
			uint32_t val2=eval(op+1,q);

			switch(tokens[op].type){
				case PLUS: return val1+val2;
				case SUB:  return val1-val2;
				case MULT: return val1*val2;
				case DEVI: return val1/val2;
				case EQ:   return val1==val2;
				case NEQ:  return val1!=val2;
				case AND:  return val1&&val2;
				case OR:   return val1||val2;
				default: printf("Unknown binary operator!\n");
			}
		}
	}
	return -1;
}

uint32_t expr(char *e, bool *success) {
	int i=0;
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	for(;i<nr_token;i++)
	{
		if((tokens[i].type==MULT)&&((i==0)||(tokens[i-1].type/10)||(tokens[i-1].type==LSB)))
			tokens[i].type=DEREF;
		else if((tokens[i].type==PLUS)&&((i==0)||(tokens[i-1].type/10)||(tokens[i-1].type==LSB)))
			tokens[i].type=POS;
		else if((tokens[i].type==SUB)&&((i==0)||(tokens[i-1].type/10)||(tokens[i-1].type==LSB)))
			tokens[i].type=NEG;
	}

	//for(i=0;i<nr_token;i++)
	//	printf("%d.type: %d  str: %s\n",i,tokens[i].type,tokens[i].str);

	*success=true;
	return eval(0,nr_token-1);
}

