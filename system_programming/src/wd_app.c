#define _POSIX_SOURCE
#include <stdio.h>		/* printf */
#include <signal.h>		/* kill */
#include <assert.h>		/* assert */
#include <unistd.h>  	/* getppid */

#include "utils.h"		/* EXIT_IF_BAD */
#include "scheduler.h"	/* SchedulerCreate */
#include "uid.h"		/* IsMatchId */


int main(int arg, char* argv[])
{
	/* validate arguments */
	
	/* extract interval */
	
	/* init wd */
	
	/* create scheduler */
	
	/* handle failure */
	
	/* add task */
	
	/* handle failure */
	
	/* run scheduler */
}

static sch_op_ty WatchDogIMP(void* param_)
{
	/* assert */
	
	/* send signal to parent */
	
	/* if parent is RIP */
		/* exec */
		/* handle failure */
	
	/* return repeat */
}

static void FuncIMP(void* unused_)
{
	(void)unused_;
}
