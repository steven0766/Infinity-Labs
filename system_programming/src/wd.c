#define _POSIX_SOURCE
#include <assert.h> /* assert */
#include <signal.h>
#include <stdlib.h>
#include <string.h> /* memmove */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"
#include "wd.h"

static pid_t g_wd_pid = 0;

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_);

int MakeMeImmortal(size_t cmd_len_, const char** cmd_, int how_often_)
{
  /* assert */

  /* init wd guard */

  /* create wd guard thread */

  /* wait for wd guard thread to be ready */

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

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_)
{
}
