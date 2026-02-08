/***********************************************************************
*Developer: Steve
*Reviewer: Rozaline
*Version:  1.0
*Date:	   03.02.26
************************************************************************/

#include <stddef.h> /* size_t */

#include "fsm.h"	/* IsInLanguage */

#define TRUE					1
#define FALSE					0

typedef enum state
{
	START = 0,
	PIT = 1,
	START_WITH_ZERO = 2,
	ACCEPT = 3
}state_ty;

int IsInLanguage(const char* bits)
{
	state_ty state_ans[4][2] = {{START_WITH_ZERO, PIT},
								{PIT, PIT},
								{ACCEPT, START_WITH_ZERO},
								{ACCEPT, START_WITH_ZERO}};
	state_ty state = START;
	
	while ('\0' != *bits)
	{
		state = state_ans[state][*bits - '0'];
		++bits;
	}
	
	return (ACCEPT == state) ? 1 : 0;
}
