#define _POSIX_SOURCE
#include <signal.h>		/* kill */
#include <assert.h>		/* assert */
#include <unistd.h>  	/* getppid */
#include <stdlib.h>		/* atoi */
#include <errno.h>		/* errno */

#include "utils.h"		/* EXIT_IF_BAD */
#include "scheduler.h"	/* SchedulerCreate */
#include "uid.h"		/* IsMatchId */

typedef struct 
{
	pid_t ppid;
	char** argv;
}wd_param_ty;

enum
{
	INTERVAL_IDX = 1,
	CLIENT_ARGV_IDX = 2,
	MIN_ARGC = 3
};

static sch_op_status_ty WatchDogTSK(void* param_);
static void EmptyFuncIMP(void* unused_);

int main(int argc, char* argv[])
{
	scheduler_ty* sch = NULL;
	uid_ty task_uid = {0};
	wd_param_ty wd_param = {0};
	int interval = 0;
	/* validate arguments */
	EXIT_IF_BAD(MIN_ARGC <= argc, 1, "Invalid arguments");
	
	/* extract interval */
	interval = atoi(argv[INTERVAL_IDX]);
	EXIT_IF_BAD(0 != interval, 1, "Invalid interval");
	
	/* init wd */
	wd_param.ppid = getppid();
	wd_param.argv = argv + CLIENT_ARGV_IDX;
	
	/* create scheduler */
	sch = SchedulerCreate();
	/* handle failure */
	EXIT_IF_BAD(NULL != sch, 1, "Failed to create scheduler");
	
	/* add task */
	task_uid = SchedulerAddTask(sch, interval, WatchDogTSK, &wd_param, EmptyFuncIMP, NULL);
	/* handle failure */
	EXIT_IF_BAD(1 != IsMatchId(invalid_uid_g, task_uid), 1, "Failed to add task");
	
	/* run scheduler */
	SchedulerRun(sch);
	
	/* handle failure */
	SchedulerDestroy(sch);
	
	/* run scheduler */
	return FAIL;
}

static sch_op_status_ty WatchDogTSK(void* param_)
{
	int status = 0;
	wd_param_ty* wd_param = (wd_param_ty*)param_;
	
	/* assert */
	assert(param_);
	
	/* send signal to parent */
	status = kill(wd_param->ppid, 0);

	/* if parent is RIP */
	if (SUCCESS != status)
	{
		/* exec */
		if (ESRCH == errno)
		{
			execvp(wd_param->argv[0], wd_param->argv);
		}
		/* handle failure */
		return SCH_NOT_REPEAT;
	}
	
	/* return repeat */
	return SCH_REPEAT;
}

static void EmptyFuncIMP(void* unused_)
{
	(void)unused_;
}
