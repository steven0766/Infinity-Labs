#define _POSIX_C_SOURCE 199506L
#include "scheduler.h"
#include "uid.h"
#include "utils.h"
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
  char** m_client_argv;
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

static void ToDefaultModeIMP(tsk_info_ty* tsk_info_);
static void ToReviveModeIMP(char** client_argv_);
static void SendSignalToParentIMP(char** client_argv_);
static void IncrementAndCheckCounterIMP(unsigned int fail_cnt_,
                                        char** client_argv_);
static void RegisterSignalHandlersIMP(void);
static void AddTaskIMP(tsk_info_ty* tsk_info_, scheduler_op_ty tsk_func_);
static void TaskCleanupIMP(void* unused_);

static volatile int g_should_stop = 0;
static volatile unsigned int g_missed_signals_cnt = 0;

int main(int argc, char* argv[])
{
  tsk_info_ty tsk_info = {0};
  scheduler_ty* sch = NULL;

  /* validate argc */
  /* parse interval */
  /* parse fail count */
  /* RegisterSignalHandlersIMP */
  /* SchedulerCreate */
  /* handle failure */
  /* init tsk_info */
  /* ToDefaultModeIMP */
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
  /* if g_should_stop */
  /* SchedulerStop */
  /* return SCH_NOT_REPEAT */
  /* return SCH_REPEAT */
}

static void ResetCounterSH(int sig_)
{
  /* (void)sig_ */
  /* atomic g_missed_signals_cnt = 0 */
}

static void ToDefaultModeIMP(tsk_info_ty* tsk_info_)
{
  /* assert */
  assert(tsk_info_);

  /* Add Task SendSignalToParentTSK */
  AddTaskIMP(tsk_info_, SendSignalToParentTSK);

  /* Add Task IncrementAndCheckCounterTSK */
  AddTaskIMP(tsk_info_, IncrementAndCheckCounterTSK);

  /* Add Task CheckStopWDTSK */
  AddTaskIMP(tsk_info_, CheckStopWDTSK);
}

static void ToReviveModeIMP(char** client_argv_)
{
  /* assert */
  assert(client_argv_);

  /* execvp(client_argv_[0], client_argv_) */
  execvp(client_argv_[0], client_argv_);

  /* EXIT_IF_BAD execvp failed */
  EXIT_IF_BAD(0, 1, "execvp failed");
}

static void SendSignalToParentIMP(char** client_argv_)
{

  /* assert */
  assert(client_argv_);

  /* kill(getppid(), SIGUSR1) */
  /* if success */
  if (0 == kill(getppid(), SIGUSR1))
  {
    /* return */
    return;
  }

  /* if ESRCH */
  if (ESRCH == errno)
  {
    /* ToReviveModeIMP */
    ToReviveModeIMP(client_argv_);
    return;
  }

  /* EXIT_IF_BAD_PERROR — other error */
  EXIT_IF_BAD_PERROR(0, 1, "failed to send SIGUSR1");
}

static void IncrementAndCheckCounterIMP(unsigned int fail_cnt_,
                                        char** client_argv_)
{
  /* assert */
  assert(client_argv_);

  /* increment g_missed_signals_cnt */
  /* if g_missed_signals_cnt >= fail_cnt_ */
  if (__atomic_add_fetch(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST) >=
      fail_cnt_)
  {
    /* ToReviveModeIMP */
    ToReviveModeIMP(client_argv_);
  }
}

static void RegisterSignalHandlersIMP(void)
{
  struct sigaction sa = {0};

  /* sigemptyset sa_mask */
  EXIT_IF_BAD(0 == sigemptyset(&sa.sa_mask), 1, "sigemptyset failed");

  /* sa_handler = ResetCounterSH */
  sa.sa_handler = ResetCounterSH;
  /* sigaction SIGUSR1 */
  EXIT_IF_BAD_PERROR(0 == sigaction(SIGUSR1, &sa, NULL), 1,
                     "failed to register SIGUSR1 handler");
}

static void AddTaskIMP(tsk_info_ty* tsk_info_, scheduler_op_ty tsk_func_)
{
  uid_ty task_id = {0};

  /* assert */
  assert(tsk_info_);
  assert(tsk_func_);

  /* SchedulerAddTask */
  task_id = SchedulerAddTask(tsk_info_->m_sch, tsk_info_->m_interval, tsk_func_,
                             tsk_info_, TaskCleanupIMP, NULL);

  /* EXIT_IF_BAD */
  EXIT_IF_BAD(!IsMatchId(invalid_uid_g, task_id), 1, "failed to add task");
}

static void TaskCleanupIMP(void* unused_)
{
  /* (void)unused_ */
  (void)unused_;
}