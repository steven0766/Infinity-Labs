#include <assert.h>		/* assert */
#include <stdio.h>		/* printf */
#include <unistd.h>		/* fork, wait, execvp */
#include <stdlib.h>		/* exit */

#include "utils.h"		/* TRUE, FALSE */

static int HandleInternalCommandIMP(const char* user_input_);
static void SpawnIMP(char* command_);
static void HandleErrorIMP(const char* error_msg_);

static int last_status = 0;

void shellRun(void)
{
    /* while 1 */
    while (TRUE)
    {
        /* input command */
        /* if IsInternalCommandIMP */
            /* break */
        
        /* spawn */
    }
}

static int HandleInternalCommandIMP(const char* user_input_)
{
    /* assert */
    assert(user_input_);
    /* if user_input == "exit" */
    if (strcmp(user_input_, "exit"))
    {
        /* return true */
        return TRUE;
    }
        
    /*return FALSE;*/
    return FALSE;
}

static void SpawnIMP(char* args_[])
{
	pid_t pid;
    
    /* assert */
    assert(args_);
    
    /* fork() to make new child proccess */
    pid = fork();
    
    /* if fork is failure */
    if (-1 == pid)
    {
        /* HandleErrorIMP */
        HandleErrorIMP("fork failed");
    }

    /* if pid == 0 */
    if (0 == pid)
    {
        /* run the command using exec */
        execvp(args_[0], args_);
        /*we are here, thus we failed*/
        /* error msg*/
        HandleErrorIMP("execvp failed");
    }
    /* else   */
    else
    {
        /* ret_wait = wait(NULL) */
            /* if ret_wait == -1*/
        if (-1 == wait(&last_status);
        {
                /* HandleErrorIMP */
            HandleErrorIMP("wait Failed");
        }
	}
}

static void HandleErrorIMP(const char* error_msg_)
{
    /* error message is not null */
    assert(error_msg_);
        /* print error message */
    /* exit */
}
