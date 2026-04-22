#define _POSIX_SOURCE
#include <assert.h> /* assert */
#include <pthread.h>
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

typedef enum
{
  NORMAL = 1,
  REVIVE = 2,
  EXIT = 3
} wd_mode_ty;

enum
{
  HOW_OFTEN_IDX = 1,
  FAIL_CNT_IDX = 2,
  CLIENT_ARGS_IDX = 3,
  INT_MAX_DIGITS = 11
};

static pid_t g_wd_pid = 0;
static pthread_t g_wd_guard_thread = 0;
static volatile size_t g_cnt = 0;
static volatile wd_mode_ty g_mode = NORMAL;

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_);

int MakeMeImmortal(size_t cmd_len_, const char** cmd_, int how_often_)
{
  /* assert */

  /* init signal mask */
  /* init wd guard info */

  /* pthread_sigmask*/

  /* create wd guard thread */

  /* sigwait */

  /* if received SIGUSR1 */
  /* return SUCCESS*/

  /* return FAIL */
}

void DoNotResussitate(void)
{
  /* assert */

  /* g_mode = EXIT */

  /* wait for KillWDAppTSK finish */

  /* join g_wd_guard_thread */

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
  /* free alloc*/

  /* if parent */
  /* g_wd_pid = pid */
  /* sem_post */
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
  /* add CheckModeAndUpdateSchTSK */

  /* return NOT_REPEAT */
}

static sch_op_status_ty SendSignalTSK(void* tsk_info_)
{
  /* assert */

  /* kill - send SIGUSR1*/

  /* return REPEAT*/
}

static sch_op_status_ty IncrementCounterTSK(void* tsk_info_)
{
  /* assert */

  /* atomic g_cnt */

  /* return REPEAT */
}

static sch_op_status_ty CheckCounterTSK(void* tsk_info_)
{
  /* if g_cnt >= FAIL_CNT_IDX*/
  /* add ReviveWDTSK with interval 0*/
  /* return NOT_REPEAT */

  /* return REPEAT */
}

static sch_op_status_ty ReviveWDTSK(void* tsk_info_)
{
  /* pid = fork */
  /* handle fail */

  /* if child */
  /* exec wd_app */

  /* if parent */
  /* g_wd_pid = pid */
  /* atomic g_cnt = 0 */
  /* add InitSchedulerTSK with interval 0 */

  /* return NOT REPEAT */
}

static sch_op_status_ty StopSchedulerTSK(void* tsk_info_)
{
  /* assert */

  /* stop scheduler */

  /* return NOT_REPEAT */
}

static sch_op_status_ty CheckModeAndUpdateSchTSK(void* tsk_info_)
{
  /* assert */

  /* CheckModeIMP*/

  /* return REPEAT */
}

static void ResetCounterSigHandlerIMP(int sig_)
{
  /* atomic g_cnt = 0 */
}

static void TerminateWDSigHandlerIMP(int sig_)
{
  /* stop scheduler */
}

static void TerminateWDSigHandlerIMP(int sig)
{
  /* g_mode = EXIT */
}

static void CheckModeIMP(tsk_info_ty* tsk_info_)
{
  /* assert */

  /* if h_mode is EXIT */
  /* stop scheduler */
}

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_)
{
}
