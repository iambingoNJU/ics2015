#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	char expr[40];
	uint32_t expr_value;

} WP;

void create_wp(char *e);
void delete_wp(int n);
void display_wp();
void check_wp();

#endif
