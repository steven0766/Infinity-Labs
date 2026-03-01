
static int IsInternalCommandIMP(const char* user_input_);
static void SpawnIMP(char* command_);
static void HandleErrorIMP(const char* error_msg_);

void shellRun(void)
{
    /* while 1 */
        /* input command */
        /* if IsInternalCommandIMP */
            /* break */
        
        /* spawn */
}

static int IsInternalCommandIMP(const char* user_input_)
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

static void SpawnIMP(char* command_)
{
    /* assert */
    /* fork() to make new child proccess */
    /* if fork is failure */
        /* HandleErrorIMP */

    /* if pid == 0 */
        /*second version splits input to tokens*/
        /* prepare char array for execv */
        /* run the command using exec */
        /*we are here, thus we failed*/
        /* error msg*/
    /* else   */
        /* ret_wait = wait(NULL) */
            /* if ret_wait == -1*/
                /* HandleErrorIMP */

}

static void HandleErrorIMP(const char* error_msg_)
{
    /* error message is not null */
        /* print error message */
    /* exit */
}
