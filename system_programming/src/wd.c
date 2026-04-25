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
  int sig = 0;

  /* assert wd is on */
  assert(0 != g_wd_app_pid);

  /* set EXIT flag */
  __atomic_store_n(&g_mode, EXIT, __ATOMIC_SEQ_CST);

  /* sigwait KillWDAppTSK to signal done */
  sigwait(&g_mask, &sig);

  /* pthread_join */
  pthread_join(g_wd_guard_thread, NULL);

  /* g_wd_app_pid = 0 */
  g_wd_app_pid = 0;
}

static void* WDGuardThread(void* wd_info_)
{
  wd_guard_info_ty* wd_info = (wd_guard_info_ty*)wd_info_;
  tsk_info_ty tsk_info = {0};
  scheduler_ty* sch = NULL;
  char** wd_argv = NULL;
  pid_t pid = 0;

  /* assert */
  assert(wd_info_);
  assert(0 == g_wd_app_pid);

  /* AllocAndInitWDArgvIMP */
  wd_argv = AllocAndInitWDArgvIMP(wd_info->m_argv_len, wd_info->m_argv,
                                  wd_info->m_interval, wd_info->m_fail_cnt);
  /* handle failure */
  RETURN_IF_BAD_CLEAN(NULL != wd_argv, NULL, "failed to alloc wd argv",
                      pthread_kill(wd_info->m_main_thread, SIGUSR2));

  /* SchedulerCreate */
  sch = SchedulerCreate();
  /* handle failure */
  RETURN_IF_BAD_CLEAN(
      NULL != sch, NULL, "failed to create scheduler",
      (FreeWDArgvIMP(wd_argv), pthread_kill(wd_info->m_main_thread, SIGUSR2)));

  /* InitSigHandlersIMP */
  /* handle failure */
  RETURN_IF_BAD_CLEAN(0 == InitSigHandlersIMP(), NULL,
                      "failed to init sig handlers",
                      (FreeWDArgvIMP(wd_argv), SchedulerDestroy(sch),
                       pthread_kill(wd_info->m_main_thread, SIGUSR2)));

  /* InitTskInfoIMP */
  InitTskInfoIMP(&tsk_info, wd_argv, sch, wd_info->m_interval,
                 wd_info->m_fail_cnt, wd_info->m_main_thread,
                 wd_info->m_sig_mask);

  /* SpawnIMP */
  pid = SpawnIMP(wd_argv);
  /* handle failure */
  RETURN_IF_BAD_CLEAN(-1 != pid, NULL, "failed to spawn wd_app",
                      (FreeWDArgvIMP(wd_argv), SchedulerDestroy(sch),
                       pthread_kill(wd_info->m_main_thread, SIGUSR2)));
  /* g_wd_app_pid = pid */
  g_wd_app_pid = pid;

  /* AddTaskIMP BuildWDTSK */
  AddTaskIMP(&tsk_info, 0, BuildWDTSK);

  /* SchedulerRun */
  SchedulerRun(sch);

  /* handle FAIL */
  /* cleanup: SchedulerDestroy, FreeWDArgvIMP */
  SchedulerDestroy(sch);
  FreeWDArgvIMP(wd_argv);

  /* return NULL */
  return NULL;
}

static sch_op_status_ty BuildWDTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;
  int sig_received = 0;

  /* assert */
  assert(tsk_info_);

  /* sigwait m_sig_mask, wait for SIGUSR1 or SIGUSR2 */
  sigwait(info->m_sig_mask, &sig_received);

  /* if SIGUSR2 */
  if (SIGUSR2 == sig_received)
  {
    /*notify main thread SIGUSR2
    return SCH_NOT_REPEAT */
    pthread_kill(info->m_main_thread, SIGUSR2);
    return SCH_NOT_REPEAT;
  }

  /* SIGUSR1 received, switch to normal state */
  /* unblock signals */
  pthread_sigmask(SIG_UNBLOCK, info->m_sig_mask, NULL);
  /* add SendSignalTSK  */
  AddTaskIMP(info, info->m_interval, SendSignalTSK);
  /* add IncrementAndCheckCounterTSK  */
  AddTaskIMP(info, info->m_interval, IncrementAndCheckCounterTSK);
  /* add CheckModeTSK */
  AddTaskIMP(info, info->m_interval, CheckModeTSK);

  /* notify main thread SIGUSR1 */
  pthread_kill(info->m_main_thread, SIGUSR1);

  /* return SCH_NOT_REPEAT */
  return SCH_NOT_REPEAT;
}

static sch_op_status_ty SendSignalTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;
  /* assert */
  assert(tsk_info_);

  /* SendSignalIMP */
  SendSignalIMP(info);

  /* return SCH_REPEAT */
  return SCH_REPEAT;
}

static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;
  /* assert */
  assert(tsk_info_);

  /* IncrementAndCheckCounterIMP */
  IncrementAndCheckCounterIMP(info);

  /* return SCH_REPEAT */
  return SCH_REPEAT;
}

static sch_op_status_ty CheckModeTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;
  /* assert */
  assert(tsk_info_);

  /* CheckModeIMP */
  CheckModeIMP(info);

  /* return SCH_REPEAT */
  return SCH_REPEAT;
}

static sch_op_status_ty KillWDAppTSK(void* tsk_info_)
{
  tsk_info_ty* info = (tsk_info_ty*)tsk_info_;

  /* assert */
  assert(tsk_info_);

  /* kill g_wd_app_pid SIGKILL */
  kill(g_wd_app_pid, SIGKILL);

  /* waitpid g_wd_app_pid */
  waitpid(g_wd_app_pid, NULL, 0);

  /* signal main thread SIGUSR1 - DoNotResuscitate */
  pthread_kill(info->m_main_thread, SIGUSR1);

  /* SchedulerStop m_sch */
  SchedulerStop(info->m_sch);

  /* return SCH_NOT_REPEAT */
  return SCH_NOT_REPEAT;
}

static void ResetCounterSH(int sig_)
{
  (void)sig_;
  /* atomic g_counter = 0 */
  __atomic_store_n(&g_cnt, 0, __ATOMIC_SEQ_CST);
}

static void SendSignalIMP(tsk_info_ty* info_)
{
  int is_sent = 0;
  /* assert */
  assert(info_);

  /* kill g_wd_app_pid SIGUSR1 */
  is_sent = kill(g_wd_app_pid, SIGUSR1);

  /* if FAIL*/
  if (FAIL == is_sent)
  {
    /* if ESRCH */
    if (ESRCH == errno)
    {
      /* SwitchToReviveModeIMP */
      SwitchToReviveModeIMP(info_);
      return;
    }

    /* EXIT_IF_BAD_PERROR */
    EXIT_IF_BAD_PERROR(0, 1, "kill failed");
  }
}

static void IncrementAndCheckCounterIMP(tsk_info_ty* info_)
{
  /* assert */
  assert(info_);

  /* atomic increment cnt */
  /* if g_cnt >= m_fail_cnt */
  if (__atomic_add_fetch(&g_cnt, 1, __ATOMIC_SEQ_CST) >=
      (size_t)info_->m_fail_cnt)
  {
    /* SwitchToReviveModeIMP */
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
  EXIT_IF_BAD(-1 != pid, 1, "SpawnIMP failed");

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
    exit(1);
  }
  /* return pid */
  return pid;
}

static void AddTaskIMP(tsk_info_ty* info_, int interval_,
                       sch_op_status_ty (*func_)(void*))
{
  uid_ty task_id = {0};

  /* assert */
  assert(info_);
  assert(func_);

  /* SchedulerAddTask */
  task_id =
      SchedulerAddTask(info_->m_sch, interval_, func_, info_, CleanupIMP, NULL);

  /* EXIT_IF_BAD */
  EXIT_IF_BAD(!IsMatchId(invalid_uid_g, task_id), 1,
              "add task to scheduler faield");
}

static int InitSignalMaskIMP(sigset_t* mask_)
{
  /* assert */
  assert(mask_);

  /* sigemptyset */
  RETURN_IF_BAD(0 == sigemptyset(mask_), 1, "sigemptyset fail");
  /* sigaddset SIGUSR1 */
  RETURN_IF_BAD(0 == sigaddset(mask_, SIGUSR1), 1, "siggaddset SIGUSR1 fail");
  /* sigaddset SIGUSR2 */
  RETURN_IF_BAD(0 == sigaddset(mask_, SIGUSR2), 1, "siggaddset SIGUSR1 fail");
  /* return SUCCESS */
  return SUCCESS;
}

static int InitSigHandlersIMP(void)
{
  struct sigaction sa = {0};
  /* sigemptyset sa_mask */
  RETURN_IF_BAD(0 == sigemptyset(&sa.sa_mask), 1, "sigemptyset fail");

  /* sa_handler = ResetCounterIMP */
  /* sigaction SIGUSR1 */
  sa.sa_handler = ResetCounterSH;
  RETURN_IF_BAD_PERROR(0 == sigaction(SIGUSR1, &sa, NULL), 1,
                       "sigaction SIGUSR1 fail");

  /* return SUCCESS */
  return SUCCESS;
}

static void InitTskInfoIMP(tsk_info_ty* info_, char** wd_argv_,
                           scheduler_ty* sch_, int interval_, int fail_cnt_,
                           pthread_t main_thread_, sigset_t* mask_)
{
  /* assert */
  assert(info_);
  assert(wd_argv_);
  assert(sch_);
  assert(0 < interval_);
  assert(0 < fail_cnt_);
  assert(mask_);

  /* set all m_ fields */
  info_->m_argv = wd_argv_;
  info_->m_sch = sch_;
  info_->m_interval = interval_;
  info_->m_fail_cnt = fail_cnt_;
  info_->m_main_thread = main_thread_;
  info_->m_sig_mask = mask_;
}

static void InitWDGuardInfoIMP(wd_guard_info_ty* info_, char** argv_,
                               size_t argv_len_, int interval_, int fail_cnt_,
                               sigset_t* mask_)
{
  /* assert */
  assert(info_);
  assert(argv_);
  assert(0 < interval_);
  assert(0 < fail_cnt_);
  assert(mask_);

  /* set all m_ fields */
  info_->m_argv = argv_;
  info_->m_argv_len = argv_len_;
  info_->m_interval = interval_;
  info_->m_fail_cnt = fail_cnt_;
  info_->m_sig_mask = mask_;
  info_->m_main_thread = pthread_self();
}

static char** AllocAndInitWDArgvIMP(size_t argv_len_, char* argv_[],
                                    int interval_, int fail_cnt_)
{
  char** wd_argv = NULL;
  char* interval = NULL;
  char* fail_cnt = NULL;

  /* assert */
  assert(argv_);
  assert(0 < interval_);
  assert(0 < argv_len_);

  /* malloc wd_argv */
  wd_argv = (char**)malloc(sizeof(char*) * (argv_len_ + REQUIRED_ARGS + 1));
  EXIT_IF_BAD(NULL != wd_argv, 1, "wd_argv malloc failed");
  /* malloc interval */
  interval = (char*)malloc(INT_MAX_DIGITS);
  EXIT_IF_BAD(NULL != interval, 1, "interval malloc failed");
  /* malloc fail_cnt */
  fail_cnt = (char*)malloc(INT_MAX_DIGITS);
  EXIT_IF_BAD(NULL != fail_cnt, 1, "fail_cnt malloc failed");

  /* wd_argv[0] = WD_APP */
  wd_argv[0] = WD_APP;
  /* sprintf interval */
  sprintf(interval, "%d", interval_);
  wd_argv[INTERVAL_IDX] = interval;
  /* sprintf fail_cnt  */
  sprintf(fail_cnt, "%d", fail_cnt_);
  wd_argv[FAIL_CNT_IDX] = fail_cnt;
  /* memmove rest */
  memmove(wd_argv + REQUIRED_ARGS, argv_, sizeof(char*) * (argv_len_ + 1));
  /* NULL terminator */
  wd_argv[REQUIRED_ARGS + argv_len_] = NULL;

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