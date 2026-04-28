#define _POSIX_C_SOURCE 199506L
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
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
  char** m_wd_argv;
  unsigned int m_fail_cnt;
  unsigned int m_interval;
  sem_t* m_sem;
  pid_t m_wd_pid;
} tsk_info_ty;

typedef struct
{
  char** m_client_argv;
  size_t m_client_argv_len;
  unsigned int m_interval;
  unsigned int m_fail_cnt;
  sem_t* m_sem;
} wd_guard_info_ty;

enum
{
  INT_MAX_DIGITS = 11,
  INTERVAL_IDX = 1,
  FAIL_CNT_IDX = 2,
  WD_ARGS_IDX = 3
};

static void* WDGuardThreadIMP(void* wd_info_);

static sch_op_status_ty CheckWDSuccessOrFailureTSK(void* tsk_info_);
static sch_op_status_ty SendSignalToWDTSK(void* tsk_info_);
static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_);
static sch_op_status_ty CheckStopWDTSK(void* tsk_info_);

static void ResetCounterSH(int sig_);

static int ToDefaultModeIMP(tsk_info_ty* tsk_info_);
static void ToReviveModeIMP(tsk_info_ty* tsk_info_);
static void ToExitModeIMP(tsk_info_ty* tsk_info_);
static void CheckWDSuccessOrFailureIMP(tsk_info_ty* tsk_info_);
static int SpawnWDIMP(tsk_info_ty* wd_argv_);
static char** AllocWDArgvIMP(wd_guard_info_ty* wd_info_);
static int InitSignalMaskIMP(sigset_t* msk_);
static int RegisterSignalHandlersIMP(void);
static int ResetSignalHandlersIMP(void);
static void CleanupOnFailureIMP(pthread_t thread_, sem_t* sem_, sigset_t* msk_);
static void NotifyFailureAndCleanupIMP(char** wd_argv_, sem_t* sem_,
                                       scheduler_ty* sch_);
static int AddTaskIMP(tsk_info_ty* tsk_info_, scheduler_op_ty tsk_func_);
static void FreeWDArgvIMP(char** wd_argv_);
static void CleanupTaskIMP(void* unused_);

static int g_is_wd_guard_ready = 0;
static pthread_t g_wd_guard_thread = 0;
static volatile int g_should_stop = 0;
static volatile unsigned int g_missed_signals_cnt = 0;

int MakeMeImmortal(size_t cmd_len_, char* cmd_[], int how_often_, int fail_cnt_)
{
  sigset_t msk = {0};
  sem_t sem = {0};
  wd_guard_info_ty wd_info = {0};
  pthread_t wd_guard_thread = 0;

  /* assert args */
  assert(cmd_);
  assert(0 < how_often_);
  assert(0 < fail_cnt_);
  assert(0 < cmd_len_);
  assert(0 == g_wd_guard_thread);

  /* init signal mask */
  RETURN_IF_BAD(0 == InitSignalMaskIMP(&msk), 1, "failed to set signal mask");

  /* block SIGUSR1 */
  RETURN_IF_BAD(0 == pthread_sigmask(SIG_BLOCK, &msk, NULL), 1,
                "failed to block signals");

  /* init semaphore */
  RETURN_IF_BAD_PERROR_CLEAN(0 == sem_init(&sem, 0, 0), 1,
                             "failed to init semaphore",
                             pthread_sigmask(SIG_UNBLOCK, &msk, NULL));

  /* init wd_info */
  wd_info.m_client_argv = cmd_;
  wd_info.m_client_argv_len = cmd_len_;
  wd_info.m_sem = &sem;
  wd_info.m_fail_cnt = fail_cnt_;
  wd_info.m_interval = how_often_;

  /* create wd_guard_thread */
  RETURN_IF_BAD_CLEAN(
      0 == pthread_create(&wd_guard_thread, NULL, WDGuardThreadIMP, &wd_info),
      1, "failed to create thread", CleanupOnFailureIMP(0, &sem, &msk));

  /* wait for thread to signal */
  RETURN_IF_BAD_PERROR_CLEAN(0 == sem_wait(&sem), 1, "sem_wait failed",
                             CleanupOnFailureIMP(wd_guard_thread, &sem, &msk));

  /* if wd guard failed */
  if (!g_is_wd_guard_ready)
  {
    /* cleanup and return fail */
    CleanupOnFailureIMP(wd_guard_thread, &sem, &msk);
    return FAIL;
  }

  /* destroy semaphore */
  RETURN_IF_BAD_PERROR_CLEAN(0 == sem_destroy(&sem), 1,
                             "failed to destroy semaphore",
                             CleanupOnFailureIMP(wd_guard_thread, &sem, &msk));

  /* update g_wd_guard_thread */
  g_wd_guard_thread = wd_guard_thread;

  return SUCCESS;
}

void DoNotResuscitate(void)
{
  sigset_t msk = {0};

  /* assert wd is on */
  assert(0 != g_wd_guard_thread);

  /* set stop flag */
  __atomic_store_n(&g_should_stop, 1, __ATOMIC_SEQ_CST);

  /* pthread_join */
  EXIT_IF_BAD_PERROR(0 == pthread_join(g_wd_guard_thread, NULL), 1,
                     "pthread_join failed");

  /* init signal mask */
  EXIT_IF_BAD(0 == InitSignalMaskIMP(&msk), 1, "failed to set signal mask");

  /* unblock SIGUSR1 */
  EXIT_IF_BAD(0 == pthread_sigmask(SIG_UNBLOCK, &msk, NULL), 1,
              "failed to unblock signals");

  /* restore signal handlers to default */
  EXIT_IF_BAD(0 == ResetSignalHandlersIMP(), 1,
              "failed to reset signal handlers");
}

static void* WDGuardThreadIMP(void* wd_info_)
{
  wd_guard_info_ty* wd_info = (wd_guard_info_ty*)wd_info_;
  scheduler_ty* sch = NULL;
  char** wd_argv = NULL;
  tsk_info_ty tsk_info = {0};
  sigset_t all_msk = {0};

  /* assert */
  assert(wd_info_);

  /* alloc wd argv */
  wd_argv = AllocWDArgvIMP(wd_info);
  RETURN_IF_BAD_CLEAN(
      NULL != wd_argv, NULL, "failed to allocate wd args",
      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, NULL));

  /* create scheduler */
  sch = SchedulerCreate();
  RETURN_IF_BAD_CLEAN(
      NULL != sch, NULL, "failed to create scheduler",
      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, NULL));

  /* init task info */
  tsk_info.m_sch = sch;
  tsk_info.m_fail_cnt = wd_info->m_fail_cnt;
  tsk_info.m_interval = wd_info->m_interval;
  tsk_info.m_wd_argv = wd_argv;
  tsk_info.m_sem = wd_info->m_sem;

  /* add CheckWDSuccessOrFailureTSK */
  RETURN_IF_BAD_CLEAN(0 == AddTaskIMP(&tsk_info, CheckWDSuccessOrFailureTSK),
                      NULL, "failed to add init task",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  /* fill mask with all signals */
  RETURN_IF_BAD_CLEAN(0 == sigfillset(&all_msk), NULL,
                      "failed to fill signal mask",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  /* block all signals */
  RETURN_IF_BAD_CLEAN(0 == pthread_sigmask(SIG_BLOCK, &all_msk, NULL), NULL,
                      "failed to block signals",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  /* unblock SIGUSR1 */
  RETURN_IF_BAD_CLEAN(0 == InitSignalMaskIMP(&all_msk), NULL,
                      "failed to fill signal mask",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  RETURN_IF_BAD_CLEAN(0 == pthread_sigmask(SIG_UNBLOCK, &all_msk, NULL), NULL,
                      "failed to block signals",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  /* register signal handler */
  RETURN_IF_BAD_CLEAN(0 == RegisterSignalHandlersIMP(), NULL,
                      "failed to set signal handlers",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  /* set counter to 1 */
  __atomic_store_n(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST);

  /* spawn wd_app */
  RETURN_IF_BAD_CLEAN(0 == SpawnWDIMP(&tsk_info), NULL, "failed to spawn wd",
                      NotifyFailureAndCleanupIMP(wd_argv, wd_info->m_sem, sch));

  /* run scheduler */
  SchedulerRun(sch);

  /* cleanup */
  SchedulerDestroy(sch);
  FreeWDArgvIMP(wd_argv);
  // TODO: should cleanup here as well, just call cleanup function (without
  // notify)
  PANIC_IF_BAD(0 == ResetSignalHandlersIMP(),
               "failed to reset signal handlers");

  return NULL;
}

static sch_op_status_ty CheckWDSuccessOrFailureTSK(void* tsk_info_)
{
  tsk_info_ty* tsk_info = (tsk_info_ty*)tsk_info_;

  /* assert */
  assert(tsk_info_);

  /* check if wd_app sent SIGUSR1 */
  CheckWDSuccessOrFailureIMP(tsk_info);

  /* return repeat */
  return SCH_REPEAT;
}

static sch_op_status_ty SendSignalToWDTSK(void* tsk_info_)
{
  tsk_info_ty* tsk_info = (tsk_info_ty*)tsk_info_;

  /* assert */
  assert(tsk_info_);

  /* send SIGUSR1 to wd_app */
  if (0 == kill(tsk_info->m_wd_pid, SIGUSR1))
  {
    return SCH_REPEAT;
  }

  /* kill failed, check ESRCH */
  EXIT_IF_BAD_PERROR(ESRCH == errno, 1, "failed to send SIGUSR1");

  /* revive wd_app */
  ToReviveModeIMP(tsk_info);

  return SCH_REPEAT;
}

static sch_op_status_ty IncrementAndCheckCounterTSK(void* tsk_info_)
{
  tsk_info_ty* tsk_info = (tsk_info_ty*)tsk_info_;

  /* assert */
  assert(tsk_info_);

  /* increment missed signals counter */
  /* if counter reached fail_cnt */
  if (__atomic_add_fetch(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST) >=
      tsk_info->m_fail_cnt)
  {
    /* revive */
    ToReviveModeIMP(tsk_info);
  }

  return SCH_REPEAT;
}

static sch_op_status_ty CheckStopWDTSK(void* tsk_info_)
{
  tsk_info_ty* tsk_info = (tsk_info_ty*)tsk_info_;

  /* assert */
  assert(tsk_info_);

  /* if g_should_stop flag is on */
  if (__atomic_load_n(&g_should_stop, __ATOMIC_SEQ_CST))
  {
    /* ToExitModeIMP */
    ToExitModeIMP(tsk_info);
    return SCH_NOT_REPEAT;
  }

  return SCH_REPEAT;
}

static void ResetCounterSH(int sig_)
{
  (void)sig_;

  /* wd_app sent SIGUSR1, reset g_missed_signals_cnt */
  __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
}

static int ToDefaultModeIMP(tsk_info_ty* tsk_info_)
{
  /* assert */
  assert(tsk_info_);

  /* clear tasks from scheduler */
  SchedulerClear(tsk_info_->m_sch);

  /* add tasks */
  RETURN_IF_BAD(0 == AddTaskIMP(tsk_info_, SendSignalToWDTSK), 1,
                "failed to add SendSignalToWDTSK");
  RETURN_IF_BAD(0 == AddTaskIMP(tsk_info_, IncrementAndCheckCounterTSK), 1,
                "failed to add IncrementAndCheckCounterTSK");
  RETURN_IF_BAD(0 == AddTaskIMP(tsk_info_, CheckStopWDTSK), 1,
                "failed to add CheckStopWDTSK");

  return SUCCESS;
}

static void ToReviveModeIMP(tsk_info_ty* tsk_info_)
{
  /* assert */
  assert(tsk_info_);

  /* zombie process */
  waitpid(tsk_info_->m_wd_pid, NULL, 0);

  /* spawn new wd_app */
  SpawnWDIMP(tsk_info_);

  /* reset g_missed_signals_cnt */
  __atomic_store_n(&g_missed_signals_cnt, 0, __ATOMIC_SEQ_CST);
}

static void ToExitModeIMP(tsk_info_ty* tsk_info_)
{
  int status = 0;
  sigset_t msk = {0};
  pid_t res = 0;

  assert(tsk_info_);

  /* send SIGUSR2 */
  status = kill(tsk_info_->m_wd_pid, SIGUSR2);
  EXIT_IF_BAD_PERROR(0 == status || ESRCH == errno, 1,
                     "failed to send SIGUSR2");

  /* block signals */
  EXIT_IF_BAD(0 == InitSignalMaskIMP(&msk), 1, "failed to init signal mask");
  EXIT_IF_BAD(0 == pthread_sigmask(SIG_BLOCK, &msk, NULL), 1,
              "failed to block signals");

  /* wait for wd_app */
  do
  {
    res = waitpid(tsk_info_->m_wd_pid, NULL, 0);
  } while (-1 == res && EINTR == errno);

  EXIT_IF_BAD_PERROR(tsk_info_->m_wd_pid == res, 1, "waitpid failed");

  /* stop scheduler */
  SchedulerStop(tsk_info_->m_sch);
}

static void CheckWDSuccessOrFailureIMP(tsk_info_ty* tsk_info_)
{
  unsigned int misses_count = 0;

  /* assert */
  assert(tsk_info_);

  /* increment g_missed_signals_cnt */
  misses_count = __atomic_fetch_add(&g_missed_signals_cnt, 1, __ATOMIC_SEQ_CST);

  if (0 == misses_count)
  {
    /* wd_app signaled successfully */
    g_is_wd_guard_ready = 1;
    /* switch to normal operation tasks */
    ToDefaultModeIMP(tsk_info_);
  }
  else if (tsk_info_->m_fail_cnt <= misses_count)
  {
    /* wd_app failed to signal within fail_cnt intervals */
    g_is_wd_guard_ready = 0;
    ToExitModeIMP(tsk_info_);
  }
  else
  {
    /* still waiting — not enough intervals passed yet */
    return;
  }

  /* sem_post for MakeMeImmortal */
  EXIT_IF_BAD_PERROR(0 == sem_post(tsk_info_->m_sem), 1, "sem_post failed");

  /* mark sem as dangling */
  tsk_info_->m_sem = BAD_MEM(sem_t*);
}

static int SpawnWDIMP(tsk_info_ty* tsk_info_)
{
  pid_t pid = 0;

  /* assert */
  assert(tsk_info_);

  /* fork */
  pid = fork();
  RETURN_IF_BAD(-1 != pid, 1, "fork failed");

  /* child */
  if (0 == pid)
  {
    /* exec wd_app */
    execvp(tsk_info_->m_wd_argv[0], tsk_info_->m_wd_argv);
    /* execvp failed */
    exit(1);
  }

  /* parent */
  /* save wd_app pid */
  tsk_info_->m_wd_pid = pid;

  return SUCCESS;
}

static int InitSignalMaskIMP(sigset_t* msk_)
{
  /* empty mask */
  RETURN_IF_BAD(0 == sigemptyset(msk_), 1, "sigemptyset failed");
  /* add SIGUSR1 */
  RETURN_IF_BAD(0 == sigaddset(msk_, SIGUSR1), 1, "sigaddset SIGUSR1 failed");
  return SUCCESS;
}

static int RegisterSignalHandlersIMP(void)
{
  struct sigaction sa = {0};

  /* empty signal mask */
  RETURN_IF_BAD(0 == sigemptyset(&sa.sa_mask), 1, "sigemptyset failed");

  /* register SIGUSR1 to ResetCounterSH */
  sa.sa_handler = ResetCounterSH;
  RETURN_IF_BAD_PERROR(0 == sigaction(SIGUSR1, &sa, NULL), 1,
                       "failed to register SIGUSR1 handler");

  return SUCCESS;
}

static int ResetSignalHandlersIMP(void)
{
  struct sigaction sa = {0};

  /* restore SIGUSR1 */
  sa.sa_handler = SIG_IGN;
  RETURN_IF_BAD(0 == sigaction(SIGUSR1, &sa, NULL), 1,
                "failed to reset SIGUSR1 handler");

  return SUCCESS;
}

static void CleanupOnFailureIMP(pthread_t thread_, sem_t* sem_, sigset_t* msk_)
{
  /* if thread created*/
  if (thread_)
  {
    /* g_should_stop = 1*/
    __atomic_store_n(&g_should_stop, 1, __ATOMIC_SEQ_CST);
    /* join */
    pthread_join(thread_, NULL);
  }

  /* unblock signal */
  pthread_sigmask(SIG_UNBLOCK, msk_, NULL);

  /* destroy semaphore */
  sem_destroy(sem_);
}

static void NotifyFailureAndCleanupIMP(char** wd_argv_, sem_t* sem_,
                                       scheduler_ty* sch_)
{
  /* g_is_wd_guard_ready = 0 */
  g_is_wd_guard_ready = 0;

  /* cleanup scheduler and argv */
  SchedulerDestroy(sch_);
  FreeWDArgvIMP(wd_argv_);

  /* reset signal handlers */
  ResetSignalHandlersIMP();

  /* wake up MakeMeImmortal */
  EXIT_IF_BAD_PERROR(0 == sem_post(sem_), 1, "sem_post failed");
}

static int AddTaskIMP(tsk_info_ty* tsk_info_, scheduler_op_ty tsk_func_)
{
  uid_ty task_id = {0};

  assert(tsk_info_);
  assert(tsk_func_);

  /* add task */
  task_id = SchedulerAddTask(tsk_info_->m_sch, tsk_info_->m_interval, tsk_func_,
                             tsk_info_, CleanupTaskIMP, NULL);

  /* verify task was added */
  RETURN_IF_BAD(!IsMatchId(invalid_uid_g, task_id), 1, "failed to add task");

  return SUCCESS;
}

static char** AllocWDArgvIMP(wd_guard_info_ty* wd_info_)
{
  char** wd_argv = NULL;
  char* interval = NULL;
  char* fail_cnt = NULL;

  assert(wd_info_);

  /* alloc argv */
  wd_argv = (char**)malloc(sizeof(char*) *
                           (wd_info_->m_client_argv_len + WD_ARGS_IDX + 1));
  RETURN_IF_BAD(NULL != wd_argv, NULL, "failed to alloc wd_argv");

  /* alloc interval */
  interval = (char*)malloc(INT_MAX_DIGITS);
  RETURN_IF_BAD_CLEAN(NULL != interval, NULL, "failed to alloc interval",
                      free(wd_argv));

  /* alloc fail_cnt */
  fail_cnt = (char*)malloc(INT_MAX_DIGITS);
  RETURN_IF_BAD_CLEAN(NULL != fail_cnt, NULL, "failed to alloc fail_cnt",
                      (free(interval), free(wd_argv)));

  /* set executable name */
  wd_argv[0] = WD_APP;

  /* set interval */
  sprintf(interval, "%d", wd_info_->m_interval);
  wd_argv[INTERVAL_IDX] = interval;

  /* set fail_cnt */
  sprintf(fail_cnt, "%d", wd_info_->m_fail_cnt);
  wd_argv[FAIL_CNT_IDX] = fail_cnt;

  /* copy client argv */
  memmove(wd_argv + WD_ARGS_IDX, wd_info_->m_client_argv,
          sizeof(char*) * (wd_info_->m_client_argv_len + 1));

  return wd_argv;
}

static void FreeWDArgvIMP(char** wd_argv_)
{
  if (NULL == wd_argv_)
  {
    return;
  }
  /* free wd_argv_ */
  free(wd_argv_[INTERVAL_IDX]);
  free(wd_argv_[FAIL_CNT_IDX]);
  free(wd_argv_);
}

static void CleanupTaskIMP(void* unused_)
{
  /* unused */
  (void)unused_;
}