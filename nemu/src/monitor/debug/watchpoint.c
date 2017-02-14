#include "monitor/watchpoint.h"
#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "nemu.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp()
{
	if(free_==NULL)
	{
		printf("New watchpoint error!\n");
		return NULL;
	}
	else
	{
		WP *temp=free_;
		free_=free_->next;
		temp->next=head;
		head=temp;
		return temp;
	}
}

void free_wp(WP *wp)
{
	//delete from head
	if(head==wp)
		head=head->next;
	else
	{
		WP *temp=head;
		while(temp->next!=wp)
		{
			if(temp==NULL)	break;
			temp=temp->next;
		}
		if(temp)
			temp->next=wp->next;
		else
		{
			printf("No such watchpoint to be deleted!\n");
			return;
		}
	}

	//insert to free_
	strcpy(wp->expr,"");
	wp->expr_value=0;
	wp->next=free_;
	free_=wp;
	return;
}

void create_wp(char *e)
{
	bool flag=false;
	WP *temp=head;
	while(temp!=NULL)
	{
		if(strcmp(temp->expr,e)==0)
		{
			flag=true;
			break;
		}
		temp=temp->next;
	}
	if(flag)
	{
		printf("This has been watched!\n");
		return;
	}

	uint32_t val=expr(e,&flag);
	if(flag)
	{
		WP *new_watchpoint=new_wp();
		if(new_watchpoint==NULL)
			assert(0);
		else
		{
			strcpy(new_watchpoint->expr,e);
			new_watchpoint->expr_value=val;
		}
	}
	else
	    printf("Invalid expression!\n");

}


void delete_wp(int n)
{
	Assert(n>=0&&n<NR_WP,"unexisted watchpoint!\n");
	WP *temp=head;
    while(temp!=NULL)
    {
	    if(temp->NO==n)
		    break;
		else
		    temp=temp->next;
	}
	if(temp)
		free_wp(temp);
	else
		printf("No watchpoint named %d!\n", n);
}


void display_wp()
{
	if(head==NULL)
	{
		printf("No watchpoint!\n");
		return;
	}
	else
	{
		WP *temp=head;
		printf("ALL watchpoints:\n""num   expr                                              value\n");
		while(temp!=NULL)
		{
			printf("%3d - %-40s\t\t%-d\n",temp->NO,temp->expr,temp->expr_value);
			temp=temp->next;
		}
	}

	return;
}

void check_wp()
{
	WP *temp=head;
	uint32_t newValue;
	bool fg,flag=false;
	while(temp!=NULL)
	{
		newValue=expr(temp->expr,&fg);
		if(newValue!=temp->expr_value)
		{
			printf("\tWatchpoint %d - expression: %s  changed at 0x%.8x:\n\t\told value: %d\n\t\tnew value: %d\n",temp->NO,temp->expr,cpu.eip,temp->expr_value,newValue);
			temp->expr_value=newValue;
			flag=true;
		}		
		temp=temp->next;
	}
	if(flag)
		nemu_state=STOP;
}

