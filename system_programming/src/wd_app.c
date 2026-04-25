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

static sch_op_status_ty SendSignalToParentTSK(void* tsk_info_);
static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_);
static sch_op_status_ty CheckStopWDTSK(void* tsk_info_);
static void ResetCounterSH(int sig_);
static void SetStopSH(int sig_);
static void SendSignalToParentIMP(tsk_info_ty* info_);
static void IncrementAndCheckCounterIMP(tsk_info_ty* info_);
static void SpawnIMP(tsk_info_ty* info_);
static void InitSignalHandlersIMP(void);
static void AddTaskIMP(tsk_info_ty* info_, scheduler_op_ty func_);
static void CleanupIMP(void* unused_);

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

  /* init tsk_info */

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

static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_)
{
  /* assert */

  /* IncrementAndCheckCounterIMP */

  /* return SCH_REPEAT */
}

static sch_op_status_ty CheckStopWDTSK(void* tsk_info_)
{
  /* assert */

  /* if g_should_stop
     SchedulerStop
    return SCH_NOT_REPEAT */

  /* return SCH_REPEAT */
}

static void ResetCounterIMP(int sig_)
{
  /* g_counter = 0 */
}

static void SetStopIMP(int sig_)
{
  /* g_should_stop = 1 */
}

static void SendSignalToParentIMP(tsk_info_ty* info_)
{
  /* assert */

  /* kill(getppid(), SIGUSR1) */
  /* if success return */

  /* if ESRCH SpawnIMP */

  /* EXIT_IF_BAD_PERROR */
}

static void IncrementAndCheckCounterIMP(tsk_info_ty* info_)
{
  /* assert */

  /* increment g_cnt */

  /* if g_cnt >= m_fail_cnt
     SpawnIMP */
}

static void SpawnIMP(tsk_info_ty* info_)
{
  /* assert */

  /* execvp(m_wd_args[0], m_wd_args) */

  /* EXIT_IF_BAD_PERROR */
}

static void InitSignalHandlersIMP(void)
{
  struct sigaction sa = {0};
  /* sigemptyset sa_mask */
  EXIT_IF_BAD(0 == sigemptyset(&sa.sa_mask), 1, "sigemptyset failed");

  /* sa_handler = ResetCounterIMP SIGUSR1 */
  sa.sa_handler = ResetCounterSH;
  EXIT_IF_BAD_PERROR(0 == sigaction(SIGUSR1, &sa, NULL), 1,
                     "sigaction SIGUSR1 failed");

  /* sa_handler = SetStopimp SIGUSR2 */
  sa.sa_handler = SetStopSH;
  EXIT_IF_BAD_PERROR(0 == sigaction(SIGUSR2, &sa, NULL), 1,
                     "sigaction SIGUSR2 failed");
}

static void AddTaskIMP(tsk_info_ty* info_, scheduler_op_ty func_)
{
  uid_ty task_id = {0};

  /* assert */
  assert(info_);
  assert(func_);

  /* SchedulerAddTask with m_interval */
  task_id = SchedulerAddTask(info_->m_sch, info_->m_interval, func_, info_,
                             CleanupIMP, NULL);

  /* EXIT_IF_BAD */
  EXIT_IF_BAD(!IsMatchId(invalid_uid_g, task_id), 1, "failed to add task");
}

static void CleanupIMP(void* unused_)
{
  /* (void)unused_ */
  (void)unused_;
}