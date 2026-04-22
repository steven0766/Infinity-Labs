#define _POSIX_SOURCE
#include <assert.h> /* assert */
#include <errno.h>  /* errno */
#include <signal.h> /* kill */
#include <stdlib.h> /* atoi */
#include <unistd.h> /* getppid */

#include "scheduler.h" /* SchedulerCreate */
#include "uid.h"       /* IsMatchId */
#include "utils.h"     /* EXIT_IF_BAD */

typedef struct
{
  char** m_wd_args;
  scheduler_ty* m_sch;
  unsigned int m_fail_cnt;
  unsigned int m_interval;
} tsk_info_ty;

enum
{
  INTERVAL_IDX = 1,
  FAIL_CNT_IDX = 2,
  CLIENT_ARGS_IDX = 3,
  MIN_ARGS = 4
};

static volatile int g_should_stop = 0;
static volatile unsigned int g_cnt = 0;

int main(int argc, char* argv[])
{
  /* validate argc */

  /* parse interval */

  /* parse fail count */

  /* InitSignalHandlersIMP */

  /* SchedulerCreate */
  /* handle failure */

  /* init tsk_info fields */

  /* AddTaskIMP SendSignalToParentTSK */
  /* AddTaskIMP IncrementAndCheckCounterTSK */
  /* AddTaskIMP CheckStopWDTSK */

  /* SchedulerRun */

  /* SchedulerDestroy */

  /* return SUCCESS */
}

static sch_op_status_ty SendSignalToParentTSK(void* tsk_info_)
{
  /* assert */

  /* SendSignalToParentIMP */

  /* return SCH_REPEAT */
}