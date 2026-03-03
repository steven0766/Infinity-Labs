#include <assert.h>		/* assert */
#include <stdio.h>		/* printf */
#include <unistd.h>		/* fork, wait, execvp */
#include <stdlib.h>		/* exit */

#include "utils.h"		/* TRUE, FALSE */

static int HandleInternalCommandIMP(const char* user_input_);
static void SpawnIMP(char* command_);
static void HandleErrorIMP(const char* error_msg_);
static void ParseArgsIMP(char* user_input_, char* args_[]);

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
    /* print error message */
    perror(error_msg_);
    /* exit */
    exit();
}

static void ParseArgsIMP(char* user_input_, char* args_[])
{
    char* token = NULL;
    size_t argc = 0;

	/* assert */
    assert(user_input_);
    assert(args_);
	
	/* devide user input to tokens */
    token = strtok(user_input_, " ");
	
	/* add akk tokens to args_ in loop */
    while (token && argc < (MAX_ARGS - 1))
    {
    	/* add token to argc */
        args_[argc++] = token;
        /* next token */
        token = strtok(NULL, " ");
    }
	/* last arg is NULL */
    args_[argc] = NULL;
}
