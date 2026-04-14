#define _POSIX_SOURCE

#include <stdio.h>      /* printf */
#include <unistd.h>     /* getpid, sleep */
#include <sys/types.h>  /* pid_t */

#include "utils.h"
#include "wd.h"

int main(int argc, char *argv[])
{
    int status = 0;

    /* make immortal */
    status = MakeMeImmortal((size_t)argc, (const char **)argv, 1);
    EXIT_IF_BAD(SUCCESS == status, 1, "MakeMeImmortal failed");
    
    printf("PID: %d\n", getpid());

    while (1){}

    return 0;
}
