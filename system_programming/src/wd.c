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

static sch_op_status_ty BuildWDTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;
  int sig_received = 0;

  /* assert */

  /* sigwait m_sig_mask -> wait for SIGUSR1 or SIGUSR2 from wd_app */

  /* if SIGUSR2 -> notify main thread SIGUSR2, return SCH_NOT_REPEAT */

  /* SIGUSR1 received — switch to normal state */
  /* unblock signals (pthread_sigmask SIG_UNBLOCK) */
  /* add SendSignalTSK (m_interval) */
  /* add IncrementAndCheckCounterTSK (m_interval) */
  /* add CheckModeTSK (m_interval) */

  /* notify main thread SIGUSR1 */

  /* return SCH_NOT_REPEAT */
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

  /* signal main thread SIGUSR1 - DoNotResuscitate */

  /* SchedulerStop m_sch */

  /* return SCH_NOT_REPEAT */
}

static void ResetCounterSH(int sig_)
{
  /* atomic g_counter = 0 */
}

static void SendSignalIMP(tsk_info_ty* info_)
{
  /* assert */

  /* kill g_wd_app_pid SIGUSR1 */
  /* if success return */

  /* if ESRCH SwitchToReviveModeIMP */

  /* EXIT_IF_BAD_PERROR */
}

static void IncrementAndCheckCounterIMP(tsk_info_ty* info_)
{
  /* assert */

  /* atomic increment cnt */

  /* if g_counter >= m_fail_cnt */
  /* SwitchToReviveModeIMP */
}

static void SwitchToReviveModeIMP(tsk_info_ty* info_)
{
  pid_t pid = 0;

  /* assert */

  /* loop: SpawnIMP until pid != -1 */

  /* g_wd_app_pid = pid */

  /* atomic store g_counter = 0 */

  /* AddTaskIMP BuildWDTSK (interval=0) */
}

static void CheckModeIMP(tsk_info_ty* info_)
{
  /* assert */

  /* if EXIT == g_mode -> AddTaskIMP KillWDAppTSK (interval=0) */
}

static pid_t SpawnIMP(char** wd_argv_)
{
  /* assert */

  /* fork */
  /* if -1 -> return -1 */
  /* if child -> execvp, exit(FAIL) */
  /* return pid */
}

static void AddTaskIMP(tsk_info_ty* info_, int interval_,
                       sch_op_status_ty (*func_)(void*))
{
  uid_ty task_id = {0};

  /* assert */

  /* SchedulerAddTask */

  /* EXIT_IF_BAD */
}

static int InitSignalMaskIMP(sigset_t* mask_)
{
  /* assert */

  /* sigemptyset */
  /* sigaddset SIGUSR1 */
  /* sigaddset SIGUSR2 */
  /* return SUCCESS */
}

static int InitSigHandlersIMP(void)
{
  /* sigemptyset sa_mask */

  /* sa_handler = ResetCounterSH */
  /* sigaction SIGUSR1 */

  /* return SUCCESS */
}

static void InitTskInfoIMP(tsk_info_ty* info_, char** wd_argv_,
                           scheduler_ty* sch_, int interval_, int fail_cnt_,
                           pthread_t main_thread_, sigset_t* mask_)
{
  /* assert */

  /* set all m_ fields */
}

static void InitWDGuardInfoIMP(wd_guard_info_ty* info_, char** argv_,
                               size_t argv_len_, int interval_, int fail_cnt_,
                               sigset_t* mask_)
{
  /* assert */

  /* set all m_ fields */
  /* m_main_thread = pthread_self() */
}