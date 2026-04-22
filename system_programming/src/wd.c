#define _POSIX_SOURCE
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "scheduler.h"
#include "uid.h"
#include "utils.h"
#include "wd.h"

#define WD_APP "./wd_app"

typedef struct
{
  scheduler_ty* m_sch;
  char** m_argv;
  int m_fail_cnt;
  int m_interval;
  pthread_t m_main_thread;
  sigset_t* m_sig_mask;
} tsk_info_ty;

typedef struct
{
  char** m_argv;
  size_t m_argv_len;
  int m_interval;
  int m_fail_cnt;
  pthread_t m_main_thread;
  sigset_t* m_sig_mask;
} wd_guard_info_ty;

typedef enum
{
  NORMAL = 0,
  EXIT = 1
} wd_mode_ty;

enum
{
  INT_MAX_DIGITS = 11,
  INTERVAL_IDX = 1,
  FAIL_CNT_IDX = 2,
  WD_APP_ARGS_IDX = 3,
  REQUIRED_ARGS = 3,
  MIN_ARGS = 4
};

static pid_t g_wd_app_pid = 0;
static sigset_t g_mask = {0};
static pthread_t g_wd_guard_thread = 0;
static volatile size_t g_cnt = 0;
static volatile wd_mode_ty g_mode = NORMAL;

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_);

int MakeMeImmortal(size_t cmd_len_, char* cmd_[], int how_often_, int fail_cnt_)
{
  wd_guard_info_ty guard_info = {0};
  int sig = 0;

  /* assert */

  /* InitSignalMaskIMP */

  /* InitWDGuardInfoIMP */

  /* block signals in main thread */

  /* create wd guard thread */

  /* sigwait: SIGUSR1=SUCCESS, SIGUSR2=FAIL */

  /* return SUCCESS/FAIL */
}

void DoNotResuscitate(void)
{
  /* assert wd is on */

  /* set EXIT flag */

  /* sigwait KillWDAppTSK to signal done */

  /* pthread_join */

  /* g_wd_app_pid = 0 */
}

static void* WDGuardThread(void* wd_args_)
{
  /* assert */

  /* AllocAndInitWDArgvIMP */
  /* handle failure SIGUSR2 */

  /* SchedulerCreate */
  /* handle failure SIGUSR2 */

  /* InitSigHandlersIMP */
  /* handle failure SIGUSR2 */

  /* InitTskInfoIMP */

  /* SpawnIMP -> g_wd_app_pid = pid */
  /* handle failure SIGUSR2 */

  /* AddTaskIMP BuildWDTSK (interval=0) */

  /* SchedulerRun */

  /* cleanup: SchedulerDestroy, FreeWDArgvIMP */

  /* return NULL */
}

static sch_op_status_ty SendSignalTSK(void* tsk_info_)
{
  /* assert */

  /* SendSignalIMP */

  /* return SCH_REPEAT */
}

static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_)
{
  /* assert */

  /* IncrementAndCheckCounterIMP */

  /* return SCH_REPEAT */
}

static sch_op_status_ty CheckModeTSK(void* tsk_info_)
{
  /* assert */

  /* CheckModeIMP */

  /* return SCH_REPEAT */
}

static sch_op_status_ty KillWDAppTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;

  /* assert */

  /* kill g_wd_app_pid SIGKILL */

  /* waitpid g_wd_app_pid */

  /* pthread_kill m_main_thread SIGUSR1 -> signal DoNotResuscitate done */

  /* SchedulerStop m_sch */

  /* return SCH_NOT_REPEAT */
}