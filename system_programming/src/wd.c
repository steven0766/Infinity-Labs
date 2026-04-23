#define _POSIX_SOURCE
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
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

static void* WDGuardThreadIMP(void* wd_info_);
static sch_op_status_ty BuildWDTSK(void* tsk_info_);
static sch_op_status_ty SendSignalTSK(void* tsk_info_);
static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_);
static sch_op_status_ty CheckModeTSK(void* tsk_info_);
static sch_op_status_ty KillWDAppTSK(void* tsk_info_);
static void ResetCounterSH(int sig_);
static void SendSignalIMP(tsk_info_ty* info_);
static void IncrementAndCheckCounterIMP(tsk_info_ty* info_);
static void SwitchToReviveModeIMP(tsk_info_ty* info_);
static void CheckModeIMP(tsk_info_ty* info_);
static pid_t SpawnIMP(char** wd_argv_);
static void AddTaskIMP(tsk_info_ty* info_, int interval_,
                       sch_op_status_ty (*func_)(void*));
static int InitSignalMaskIMP(sigset_t* mask_);
static int InitSigHandlersIMP(void);
static void InitTskInfoIMP(tsk_info_ty* info_, char** wd_argv_,
                           scheduler_ty* sch_, int interval_, int fail_cnt_,
                           pthread_t main_thread_, sigset_t* mask_);
static void InitWDGuardInfoIMP(wd_guard_info_ty* info_, char** argv_,
                               size_t argv_len_, int interval_, int fail_cnt_,
                               sigset_t* mask_);
static char** AllocAndInitWDArgvIMP(size_t argv_len_, char* argv_[],
                                    int interval_, int fail_cnt_);
static void FreeWDArgvIMP(char** wd_argv_);
static void CleanupIMP(void* unused_);

int MakeMeImmortal(size_t cmd_len_, char* cmd_[], int how_often_, int fail_cnt_)
{
  wd_guard_info_ty guard_info = {0};
  int sig = 0;

  /* assert */

  /* InitSignalMaskIMP */

  /* InitWDGuardInfoIMP */

  /* block signals in main thread */

  /* create wd guard thread */

  /* sigwait */

  /* if SIGUSR1
     return SUCCESS */
  /* if SIGUSR2
     return FAIL*/
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
  /* handle failure */

  /* SchedulerCreate */
  /* handle failure */

  /* InitSigHandlersIMP */
  /* handle failure */

  /* InitTskInfoIMP */

  /* SpawnIMP */
  /* g_wd_app_pid = pid */
  /* handle failure */

  /* AddTaskIMP BuildWDTSK */

  /* SchedulerRun */

  /* cleanup: SchedulerDestroy, FreeWDArgvIMP */

  /* return NULL */
}

static sch_op_status_ty BuildWDTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;
  int sig_received = 0;

  /* assert */

  /* sigwait m_sig_mask, wait for SIGUSR1 or SIGUSR2 */

  /* if SIGUSR2
     notify main thread SIGUSR2
     return SCH_NOT_REPEAT */

  /* SIGUSR1 received, switch to normal state */
  /* unblock signals */
  /* add SendSignalTSK  */
  /* add IncrementAndCheckCounterTSK  */
  /* add CheckModeTSK */

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

static void ResetCounterIMP(int sig_)
{
  /* atomic g_counter = 0 */
}

static void SendSignalIMP(tsk_info_ty* info_)
{
  /* assert */

  /* kill g_wd_app_pid SIGUSR1 */

  /* if success
     return */

  /* if ESRCH
     SwitchToReviveModeIMP */

  /* EXIT_IF_BAD_PERROR */
}

static void IncrementAndCheckCounterIMP(tsk_info_ty* info_)
{
  /* assert */
  assert(info_);

  /* atomic increment cnt */
  __atomic_add_fetch(&g_cnt, 1, __ATOMIC_SEQ_CST);

  /* if g_cnt >= m_fail_cnt */
  if (g_cnt >= info_->m_fail_cnt)
  {
    /*   SwitchToReviveModeIMP */
    SwitchToReviveModeIMP(info_);
  }
}

static void SwitchToReviveModeIMP(tsk_info_ty* info_)
{
  pid_t pid = 0;

  /* assert */
  assert(info_);

  /* SpawnIMP */
  pid = SpawnIMP(info_);
  /* exit if fork is -1 */
  if (-1 == pid)
  {
    exit(1);
  }

  /* g_wd_app_pid = pid */
  g_wd_app_pid = pid;

  /* atomic store g_counter = 0 */
  __atomic_store_n(&g_cnt, 0, __ATOMIC_SEQ_CST);
  /* AddTaskIMP BuildWDTSK (interval=0) */
  AddTaskIMP(info_, 0, BuildWDTSK);
}

static void CheckModeIMP(tsk_info_ty* info_)
{
  /* assert */
  assert(info_);

  /* if EXIT == g_mode */
  if (EXIT == g_mode)
  {
    /* AddTaskIMP KillWDAppTSK (interval=0) */
    AddTaskIMP(info_, 0, KillWDAppTSK);
  }
}

static pid_t SpawnIMP(char** wd_argv_)
{
  pid_t pid = 0;
  /* assert */
  assert(wd_argv_);

  /* fork */
  pid = fork();
  /* if -1 */
  if (-1 == pid)
  {
    /* return -1 */
    return -1;
  }
  /* if child*/
  if (0 == pid)
  {
    /* execvp, exit(FAIL) */
    execvp(WD_APP, wd_argv_);
  }
  /* return pid */
  return pid;
}

static void AddTaskIMP(tsk_info_ty* info_, int interval_,
                       sch_op_status_ty (*func_)(void*))
{
  uid_ty task_id = {0};
  void* empty = NULL;

  /* assert */
  assert(info_);
  assert(0 < interval_);
  assert(func_);

  /* SchedulerAddTask */
  task_id = SchedulerAddTask(info_->m_sch, interval_, func_, info_->m_argv,
                             CleanupIMP, empty);

  /* EXIT_IF_BAD */
  EXIT_IF_BAD(TaskIsMatch(invalid_uid_g, task_id), 1,
              "add task to scheduler faield");
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

  /* sa_handler = ResetCounterIMP */
  /* sigaction SIGUSR1 */

  /* return SUCCESS */
}

static void InitTskInfoIMP(tsk_info_ty* info_, char** wd_argv_,
                           scheduler_ty* sch_, int interval_, int fail_cnt_,
                           pthread_t main_thread_, sigset_t* mask_)
{
  /* assert */
  assert(wd_argv_);
  assert(sch_);
  assert(0 <= interval_);

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

static char** AllocAndInitWDArgvIMP(size_t argv_len_, char* argv_[],
                                    int interval_, int fail_cnt_)
{
  char** wd_argv = NULL;
  char* interval = NULL;
  char* fail_cnt = NULL;

  /* assert */
  assert(argv_);
  assert(0 > interval);
  assert(2 > argv_len_);

  /* malloc wd_argv */
  wd_argv = (char**)malloc(sizeof(char*) * (argv_len_ + REQUIRED_ARGS + 1));
  EXIT_IF_BAD(NULL != wd_argv, 1, "wd_argv malloc failed");
  /* malloc interval */
  interval = (char*)malloc((sizeof(char*)));
  EXIT_IF_BAD(NULL != interval, 1, "interval malloc failed");
  /* malloc fail_cnt */
  fail_cnt = (char*)malloc((sizeof(char*)));
  EXIT_IF_BAD(NULL != fail_cnt, 1, "fail_cnt malloc failed");

  /* wd_argv[0] = WD_APP */
  wd_argv[0] = WD_APP;
  /* sprintf interval */
  sprintf(interval, "%d", interval_);
  wd_argv[1] = interval;
  /* sprintf fail_cnt  */
  sprintf(fail_cnt, "%d", fail_cnt_);
  wd_argv[2] = fail_cnt;
  /* memmove rest */
  memmove(wd_argv + 3, argv_, argv_len_);
  /* NULL terminator */
  wd_argv[3 + argv_len_] = NULL;

  /* return wd_argv */
  return wd_argv;
}

static void FreeWDArgvIMP(char** wd_argv_)
{
  /* assert */
  assert(wd_argv_);

  /* free wd_argv_[INTERVAL_IDX] */
  free(wd_argv_[INTERVAL_IDX]);
  /* free wd_argv_[FAIL_CNT_IDX] */
  free(wd_argv_[FAIL_CNT_IDX]);
  /* free wd_argv_ */
  free(wd_argv_);
}

static void CleanupIMP(void* unused_)
{
  /* (void)unused_ */
  (void)unused_;
}