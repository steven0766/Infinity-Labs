#define _POSIX_SOURCE
#include <stdio.h>		/* printf */
#include <signal.h>		/* kill */
#include <assert.h>		/* assert */
#include <unistd.h>  	/* getppid */

#include "utils.h"		/* EXIT_IF_BAD */
#include "scheduler.h"	/* SchedulerCreate */
#include "uid.h"		/* IsMatchId */

typedef struct 
{
	pid_t ppid;
	char** argv;
}wd_param_ty;

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
	int status = 0;
	wd_param_ty* wd_param = (ws_param_ty*)param_;
	
	/* assert */
	assert(param_);
	
	/* send signal to parent */
	status = kill(wd_param-<ppid, 0);
	
	/* if parent is RIP */
	if (FAIL == status)
	{
		/* exec */
		execvp(ws_param->argv[0], ws_param->argv);
		/* handle failure */
		return SCH_NOT_REPEAT;
	}
	
	/* return repeat */
	return SCH_REPEAT;
}

static void FuncIMP(void* unused_)
{
	(void)unused_;
}
