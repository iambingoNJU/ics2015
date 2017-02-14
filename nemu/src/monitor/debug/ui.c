#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "memory/cache1.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

extern long long with_cache_time, no_cache_time;
void cpu_exec(uint32_t);
void print_stackframe();

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

//static shows that this function can only be called within this file.
static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

//Added
static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_bt(char *args);

static int cmd_cache1(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
	{ "si", "Execute specific number of instructions determined by argument. \n\tExample: si [n] #execute si command for n times(Default=1).", cmd_si },
	{ "info", "To show information about registers or watchpoints according to argument.\n\tExample: info r #show information about registers.\
			  \n\t         info w #show information about watchpoints.", cmd_info },
	{ "x", "To show information of memory in hexidecimal.\n\tExample: x n expr #calculate the result of expr, and then show n bytes memory information"
		"with the result as begining address in hex.", cmd_x },
	{ "p", "To calculate a expression.",cmd_p},
	{ "w", "To create a new watchpoint.",cmd_w},
	{ "d", "To delete a watchpoint.",cmd_d},
	{ "bt", "To print information about stack frame.",cmd_bt},
	{ "cache1", "To show information about cache1.",cmd_cache1},

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args)
{
	char *arg =strtok(NULL," ");
	if(arg==NULL)
		cpu_exec(1);
	else
	{
		int num=0;
		sscanf(arg,"%d",&num);
		if(num>0)
			cpu_exec(num);
	}

	return 0;
}

static int cmd_info(char *args)
{
	char *arg=strtok(NULL," ");
	if(arg==NULL)
		printf("This function needs argument!\nYou can get help by 'help info'.\n");
	else if(strcmp(arg,"r")==0)
	{
		int i;
		for(i=R_EAX;i<=R_EDI;i++)
		{
			printf("%s\t\t0x%.8x\t\t%u\n",regsl[i], cpu.gpr[i]._32, cpu.gpr[i]._32);
		}			
		printf("%s\t\t0x%.8x\t\t%u\n","eip",cpu.eip, cpu.eip);
		printf("%s\t\t0x%.8x\t\t%u\n","CR0",cpu.cr0.val, cpu.cr0.val);
		printf("%s\t\tbase: 0x%.8x \tlimit: 0x%.4x\n","GDTR",cpu.GDTR.base, cpu.GDTR.limit);
		for(i=R_ES;i<=R_DS;i++)
			printf("%s\t\tbase: 0x%.8x \tlimit:0x%.8x\tval:0x%.4x\n",sregw[i], cpu.sreg[i].base, cpu.sreg[i].limit, cpu.sreg[i].val);
	}
	else if(strcmp(arg,"w")==0)
	{
		display_wp();
	}
	else
		printf("No such argument!\nYou can get help by 'help info'.\n");
	return 0;
}

static int cmd_x(char *args)
{
	int num_of_4b=0,addr=0;
	sscanf(args,"%d",&num_of_4b);	
	assert(num_of_4b>0 && num_of_4b<300);

	char *num=strtok(NULL," ");
	char *expression=args+strlen(num)+1;	
	bool success;
	addr=expr(expression,&success);
	//assert(addr>0&&addr<0xffffffff);
	
	if(success)
	{
		int i=0;
		while(i<4*num_of_4b)
		{
			printf("  %.2x",swaddr_read(addr, 1, R_DS));
			addr++;
			i++;
			if(i%4==0)
				printf("\t");
			if(i%8==0)
				printf("\n");
		}
		printf("\n");
	}
	else
		printf("Invalid expression!\n");

	return 0;
}

static int cmd_p(char *args)
{
	uint32_t result=0;
	bool success;
	result=expr(args,&success);
	if(success)
		printf("result: %d (Hex:0x%x)\n", result,result);
	else
		printf("Invalid expression!\n");

	return 0;
}

static int cmd_w(char *args)
{
	create_wp(args);
	return 0;
}

static int cmd_d(char *args)
{
	int n;
	sscanf(args,"%d",&n);
	delete_wp(n);
	return 0;
}

static int cmd_bt(char *args)
{
	print_stackframe();
	return 0;
}

static int cmd_cache1(char *args)
{
	int k;
	cache1_addr temp;
	if(strcmp(args,"all")==0)
	{
		int i,j;
		for(i=0;i<CACHE1_NR_SET;i++)
			for(j=0;j<CACHE1_SET_SIZE;j++)
				if(cache1[i][j].valid)
				{
					temp.row=i;	temp.col=0;	temp.tag=cache1[i][j].tag;
					printf("Tag: 0x%x  Set: 0x%x  Line in set: 0x%x  Addr begins at: 0x%x\n",cache1[i][j].tag,i,j,temp.addr);
					printf("Cache data: \n");
					for(k=0;k<CACHE1_BLOCK_SIZE;k++)
					{
						printf("%.2x ",cache1[i][j].data[k]);
						if((k+1)%4==0)	printf("\t");
						if((k+1)%16==0)	printf("\n");
					}
					printf("\n");
				}
	}
	else
	{
		temp.addr=strtol(args,NULL,0);
		int j;
		for(j=0;j<CACHE1_SET_SIZE;j++)
			if(cache1[temp.row][j].valid && (cache1[temp.row][j].tag==temp.tag))
			{
				printf("Tag: 0x%x  Set: 0x%x  Line in set: 0x%x  Addr begins at: 0x%x\n",cache1[temp.row][j].tag,temp.row,j,temp.addr);
				printf("Cache data: \n");
				for(k=0;k<CACHE1_BLOCK_SIZE;k++)
				{
					printf("%.2x ",cache1[temp.row][j].data[k]);
					if((k+1)%4==0)	printf("\t");
					if((k+1)%16==0)	printf("\n");
				}
				printf("\n");
				break;
			}

		if(j==CACHE1_SET_SIZE)
			printf("Cache has no copy of address 0x%x.\n",temp.addr);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }

		//printf("All time: %lld(Has cache), %lld(No cache).\nSaved time: %lld\n",with_cache_time, no_cache_time, no_cache_time-with_cache_time);
	}
}
