#include <assert.h>		/* assert */
#include <stdio.h>		/* printf */
#include <unistd.h>		/* fork, wait, execvp */
#include <stdlib.h>		/* exit */
#include <sys/wait.h>	/* wait */
#include <string.h>		/* strcmp */

#include "utils.h"		/* TRUE, FALSE */

static int HandleInternalCommandIMP(const char* user_input_);
static void SpawnIMP(char** args_);
static void HandleErrorIMP(const char* error_msg_);
static void ParseArgsIMP(char* user_input_, char* args_[]);
static void ReadInputIMP(char* buffer_);

enum
{
    MAX_LEN = 256,
    MAX_ARGS = 8
};

static int last_status = 0;

void ShellRun(void)
{
	char user_input[MAX_LEN];
    char* args[MAX_ARGS];
    
    /* while 1 */
    while (TRUE)
    {
        /* input command */
        ReadInputIMP(user_input);
        
        ParseArgsIMP(user_input, args);
        /* if IsInternalCommandIMP */
        if (TRUE == HandleInternalCommandIMP(args[0]))
        {
        	/* continue */
            continue;
        }
        
        /* spawn */
        SpawnIMP(args);
    }
}

static int HandleInternalCommandIMP(const char* user_input_)
{
	/* if input is NULL */
	if (!user_input_)
	{
		return TRUE;
	}
    /* if user_input == "exit" */
    if (0 == strcmp(user_input_, "exit"))
    {
        /* exit(0) */
        exit(0);
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
        if (-1 == wait(&last_status))
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
    exit(1);
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

static void ReadInputIMP(char* buffer_)
{
	/* assert */
    assert(buffer_);
	/* return FAIL if input is empty */
    if (NULL == fgets(buffer_, MAX_LEN, stdin))
    {
        HandleErrorIMP("fail to read input");
    }
	/* remove \n */
    buffer_[strcspn(buffer_, "\n")] = '\0';
}
