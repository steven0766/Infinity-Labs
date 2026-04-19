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
  pid_t pid = 0;
  char** wd_app;

  int pipe_fds[2];
  char buffer;

  /* validate arguments */
  EXIT_IF_BAD(0 < cmd_len_, 1, "Invalid arguments from wd lib");
  EXIT_IF_BAD(NULL != cmd_, 1, "invalid command");
  EXIT_IF_BAD(0 < how_often_, 1, "Invalid interval");

  /* create pipe */
  pipe(pipe_fds);

  /* if watchdog already alive */
  if (0 != g_wd_pid)
  {
    /* return FAIL */
    return FAIL;
  }
  /* build argv for wd_app */
  wd_app = InitWDArgsIMP(cmd_len_, cmd_, how_often_, pipe_fds[1]);
  EXIT_IF_BAD(NULL != wd_app, 1, "Failed to init");

  /* fork */
  pid = fork();
  /* if fork failed */
  if (-1 == pid)
  {
    /* free resources */
    free(wd_app[2]);
    free(wd_app[1]);
    free(wd_app);
    return FAIL;
  }

  /* if child */
  else if (0 == pid)
  {
  	EXIT_IF_BAD(0 == close(pipe_fds[0]), 1, "Child failed to close read pipe");
    /* exec wd_app */
    execvp("./wd_app", wd_app);
    /* handle FAIL */
    free(wd_app[2]);
    free(wd_app[1]);
    free(wd_app);
    /*write FAIL on pipe*/
    write(pipe_fds[1], "1", 1);
    return FAIL;
  }

  /* parent saves wd pid */
  else
  {
  	EXIT_IF_BAD(0 == close(pipe_fds[1]), 1, "Parent failed to close write pipe");
    /*read pipe*/
    read(pipe_fds[0], &buffer, 1);
    EXIT_IF_BAD(0 == close(pipe_fds[0]), 1, "Failed to close read pipe");
    free(wd_app[2]);
    free(wd_app[1]);
    free(wd_app);
    /* update g_wd_pd */
    g_wd_pid = pid;
    /* return SUCCESS */
    if ('1' == buffer)
    {
      return FAIL;
    }
    return SUCCESS;
  }
}

void DoNotResussitate(void)
{
  int status = 0;
  /* if watchdog alive */
  if (0 != g_wd_pid)
  {
    /* kill watchdog */
    status = kill(g_wd_pid, SIGTERM);
    EXIT_IF_BAD(0 == status, 1, "Failed to kill watchdog");
    status = waitpid(g_wd_pid, NULL, 0);
    EXIT_IF_BAD(g_wd_pid == status, 1, "waitpid failed");
    /* reset g_wd_pid */
    g_wd_pid = 0;
  }
}

static char** InitWDArgsIMP(size_t cmd_len_, const char** cmd_, int how_often_,
                            int write_fd_)
{
  char* interval = NULL;
  char** wd_argv = NULL;
  char* write = NULL;
  int i = 0;

  /* assert */
  assert(cmd_);
  assert(0 < how_often_);

  /* alloc wd_argv */
  wd_argv = (char**)malloc(sizeof(char*) * (cmd_len_ + 4));
  EXIT_IF_BAD(NULL != wd_argv, 1, "Failed to alloc wd_argv");

  /* alloc interval */
  interval = (char*)malloc(sizeof(char) * 11);
  EXIT_IF_BAD(NULL != interval, 1, "Failed to alloc interval");

  /* alloc write fd */
  write = (char*)malloc(sizeof(char) * 11);
  EXIT_IF_BAD(NULL != write, 1, "Failed to alloc write fd");

  /* init wd_argv */
  wd_argv[0] = "./wd_app";
  sprintf(interval, "%d", how_often_);
  wd_argv[1] = interval;
  printf("visited\n");
  sprintf(write, "%d", write_fd_);
  wd_argv[2] = write;
  memmove(wd_argv + 3, cmd_, sizeof(char*) * cmd_len_ + 1);
  wd_argv[cmd_len_ + 3] = NULL;

  return wd_argv;
}
