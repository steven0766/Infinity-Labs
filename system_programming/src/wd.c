#define _POSIX_SOURCE
#include <assert.h> /* assert */
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h> /* memmove */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "scheduler.h"
#include "utils.h"
#include "wd.h"

typedef struct
{
  scheduler_ty* sch;
  char** wd_cmd;
  int fail_cnt;
  int interval;
} tsk_info_ty;

typedef struct
{
  sem_t* sem;
  char** cmd;
  size_t cmd_len;
  int how_often;
  int fail_cnt;
} wd_guard_info_ty;

enum
{
  HOW_OFTEN_IDX = 1,
  FAIL_CNT_IDX = 2,
  CLIENT_ARGS_IDX = 3,
  INT_MAX_DIGITS = 11
};

static pid_t g_wd_pid = 0;

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_);

int MakeMeImmortal(size_t cmd_len_, const char** cmd_, int how_often_)
{
  /* assert */

  /* open semaphore */

  /* init wd guard info */

  /* create wd guard thread */

  /* sem_wait till thread signals ready */

  /* return SUCCESS/FAIL */
}

void DoNotResussitate(void)
{
  /* assert */

  /* signal thread to stop scheduler with SIGUSR2 */
  /* wait for thread to finish */

  /* kill thread */
  /* waitpid */

  /* g_wd_pid = 0*/
}

static void* WDGuardThread(void* wd_args_)
{
  /* assert */

  /* set signals:
      1. SIGUSR1 with ResetCounterSigHandleIMP
      2. SIGUSR2 with TerminateWDSigHandlerIMP*/

  /* alloc wd_cmd */

  /* create scheduler */
  /* add InitSchedulerTSK */
  /* init tsk_info */

  /* pid = fork */

  /* handle fork failed*/

  /* if child */
  /* exec wd_app */
  /* handle fail */
  /* free ar*/

  /* if parent */
  /* g_wd_pid = pid */
  /* wait for wd_app */
  /* run scheduler */
  /* destroy scheduler */
  /* free alloc */
}

static sch_op_status_ty InitSchedulerTSK(void* tsk_info_)
{
  /* assert */

  /* add SendSignalTSK */
  /* add IncrementCounterTSK*/
  /* add CheckCounterTSK */

  /* return NOT_REPEAT */
}

static sch_op_status_ty SendSignalTSK(void* tsk_info_)
{
  /* assert */

  /* kill - send SIGUSR1*/

  /* return REPEAT*/
}

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_)
{
}
