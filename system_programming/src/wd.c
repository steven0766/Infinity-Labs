#include "wd.h"

static pid_t g_wd_pid = 0;

int MakeMeImmortal(size_t cmd_len, const char **cmd, int how_often)
{
    /* validate arguments */

    /* if watchdog already alive */
        /* return SUCCESS */

    /* build argv for wd_app */

    /* fork */
    /* if fork failed */
        /* free resources */
        /* return FAIL */

    /* if child */
        /* exec wd_app */
        /* if exec failed -> _exit(FAIL) */

    /* parent saves wd pid */

    /* free temporary resources */

    /* return SUCCESS */
}

void DoNotResussitate(void)
{
    /* if watchdog alive */
        /* kill watchdog */
        /* reset pid */
}
