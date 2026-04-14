#define _POSIX_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>		/* memmove */
#include <assert.h>		/* assert */

#include "wd.h"
#include "utils.h"

static pid_t g_wd_pid = 0;

static char** InitWDArgsIMP(size_t cmd_len_, const char **cmd_, int how_often_);

int MakeMeImmortal(size_t cmd_len_, const char **cmd_, int how_often_)
{
	pid_t pid = 0;
	char** wd_app;
	
    /* validate arguments */
    EXIT_IF_BAD(0 < cmd_len_, 1, "Invalid arguments from wd lib");
    EXIT_IF_BAD(NULL != cmd_, 1, "invalid command");
    EXIT_IF_BAD(0 < how_often_, 1, "Invalid interval");

    /* if watchdog already alive */
    if (0 != g_wd_pid)
    {
        /* return FAIL */
        return FAIL;
    }

    /* build argv for wd_app */
    wd_app = InitWDArgsIMP(cmd_len_, cmd_, how_often_);
    EXIT_IF_BAD(NULL != wd_app, 1, "Failed to init");

    /* fork */
    pid = fork();
    /* if fork failed */
    if (-1 == pid)
    {
    	/* free resources */
		free(wd_app[1]);
		free(wd_app);
        /* return FAIL */
        return FAIL;
    }

    /* if child */
    else if (0 == pid)
    {
        /* exec wd_app */
        execvp("./wd_app", wd_app);
        /* handle FAIL */
        free(wd_app[1]);
		free(wd_app);
        return FAIL;
    }

    /* parent saves wd pid */
	else
	{
		free(wd_app[1]);
		free(wd_app);
    	/* update g_wd_pd */
    	g_wd_pid = pid;
    	/* return SUCCESS */
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
        status = kill(g_wd_pid, SIGKILL);
        EXIT_IF_BAD(0 == status, 1, "Failed to kill watchdog");
        /* reset g_wd_pid */
    	g_wd_pid = 0;
    }
}

static char** InitWDArgsIMP(size_t cmd_len_, const char **cmd_, int how_often_)
{
	char* interval = NULL;
	char** wd_argv = NULL;
	
	/* assert */
	assert(cmd_);
	assert(0 < how_often_);
	
	/* alloc wd_argv */
	wd_argv = (char**)malloc(sizeof(char*) * (cmd_len_ + 3));
	EXIT_IF_BAD(NULL != wd_argv, 1, "Failed to alloc wd_argv");
	
	/* alloc interval */
	interval = (char*)malloc(sizeof(char) * 11);
	EXIT_IF_BAD(NULL != interval, 1, "Failed to alloc interval");
	
	/* init wd_argv */
	wd_argv[0] = "./wd_app";
	sprintf(interval, "%d", how_often_);
	wd_argv[1] = interval;
	memmove(wd_argv + 2, cmd_, sizeof(char*) * cmd_len_);
	wd_argv[cmd_len_ + 2] = NULL;
	
	return wd_argv;
}
