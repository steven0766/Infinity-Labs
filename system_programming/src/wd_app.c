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
  tsk_info_ty tsk_info = {0};
  scheduler_ty* sch = NULL;

  /* validate argc */
  EXIT_IF_BAD(MIN_ARGS <= argc, 1, "invalid arguments");

  /* parse interval */
  tsk_info.m_interval = atoi(argv[INTERVAL_IDX]);
  EXIT_IF_BAD(0 < tsk_info.m_interval, 1, "invalid interval");

  /* parse fail cnt */
  tsk_info.m_fail_cnt = atoi(argv[FAIL_CNT_IDX]);
  EXIT_IF_BAD(0 < tsk_info.m_fail_cnt, 1, "invalid fail count");

  /* InitSignalHandlersIMP */
  InitSignalHandlersIMP();

  /* SchedulerCreate */
  sch = SchedulerCreate();
  /* handle failure */
  EXIT_IF_BAD(NULL != sch, 1, "failed to create scheduler");

  /* init tsk_info */
  tsk_info.m_wd_args = argv + CLIENT_ARGS_IDX;
  tsk_info.m_sch = sch;

  /* AddTaskIMP SendSignalToParentTSK */
  AddTaskIMP(&tsk_info, SendSignalToParentTSK);
  /* AddTaskIMP IncrementAndCheckCounterTSK */
  AddTaskIMP(&tsk_info, IncrementAndCheckCounterTSK);
  /* AddTaskIMP CheckStopWDTSK */
  AddTaskIMP(&tsk_info, CheckStopWDTSK);

  /* SchedulerRun */
  SchedulerRun(sch);

  /* SchedulerDestroy */
  SchedulerDestroy(sch);

  /* return SUCCESS */
  return SUCCESS;
}

static sch_op_status_ty SendSignalToParentTSK(void* tsk_info_)
{
  /* assert */
  assert(tsk_info_);

  /* SendSignalToParentIMP */
  SendSignalToParentIMP((tsk_info_ty*)tsk_info_);

  /* return SCH_REPEAT */
  return SCH_REPEAT;
}

static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_)
{
  /* assert */
  assert(tsk_info_);

  /* IncrementAndCheckCounterIMP */
  IncrementAndCheckCounterIMP((tsk_info_ty*)tsk_info_);

  /* return SCH_REPEAT */
  return SCH_REPEAT;
}

static sch_op_status_ty CheckStopWDTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;

  /* assert */
  assert(tsk_info_);

  /* if g_should_stop */
  if (__atomic_load_n(&g_should_stop, __ATOMIC_SEQ_CST))
  {
    /* SchedulerStop */
    SchedulerStop(info->m_sch);
    /* return SCH_NOT_REPEAT */
    return SCH_NOT_REPEAT;
  }

  /* return SCH_REPEAT */
  return SCH_REPEAT;
}

static void ResetCounterSH(int sig_)
{
  (void)sig_;

  /* g_counter = 0 */
  __atomic_store_n(&g_cnt, 0, __ATOMIC_SEQ_CST);
}

static void SetStopSH(int sig_)
{
  (void)sig_;

  /* g_should_stop = 1 */
  __atomic_store_n(&g_should_stop, 1, __ATOMIC_SEQ_CST);
}

static void SendSignalToParentIMP(tsk_info_ty* info_)
{
  /* assert */
  assert(info_);

  /* kill(getppid(), SIGUSR1) */
  /* if success return */
  if (0 == kill(getppid(), SIGUSR1))
  {
    return;
  }

  /* if ESRCH SpawnIMP */
  if (ESRCH == errno)
  {
    SpawnIMP(info_);
    return;
  }

  /* EXIT_IF_BAD_PERROR */
  EXIT_IF_BAD_PERROR(0, 1, "kill failed");
}

static void IncrementAndCheckCounterIMP(tsk_info_ty* info_)
{
  /* assert */
  assert(info_);

  /* increment g_cnt */
  /* if g_cnt >= m_fail_cnt */
  if (__atomic_add_fetch(&g_cnt, 1, __ATOMIC_SEQ_CST) >= info_->m_fail_cnt)
  {
    /*SpawnIMP */
    SpawnIMP(info_);
  }
}

static void SpawnIMP(tsk_info_ty* info_)
{
  /* assert */
  assert(info_);

  /* execvp(m_wd_args[0], m_wd_args) */
  execvp(info_->m_wd_args[0], info_->m_wd_args);

  /* EXIT_IF_BAD_PERROR */
  EXIT_IF_BAD_PERROR(0, 1, "execvp failed");
}

static void InitSignalHandlersIMP(void)
{
  struct sigaction sa = {0};
  /* sigemptyset sa_mask */
  EXIT_IF_BAD(0 == sigemptyset(&sa.sa_mask), 1, "sigemptyset failed");

  /* sa_handler = ResetCounterSH SIGUSR1 */
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