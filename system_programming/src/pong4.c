#define _POSIX_C_SOURCE 200809L

#include <stdio.h>      /* printf */
#include <signal.h>     /* sigaction, SIGUSR1, SIGUSR2 */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* pause, kill, getpid */
#include <stdlib.h>     /* exit, strtol */
#include <errno.h>      /* errno */

#include "utils.h"

static void SignalHandlerIMP(int sig_);

static volatile sig_atomic_t was_signal_receive = 0;

int main(int argc, char *argv[])
{
    struct sigaction sa = {0};
    char *endptr = NULL;
    long ping_pid = 0;

    if (2 != argc)
    {
        printf("Usage: %s <ping_pid>\n", argv[0]);
        exit(1);
    }

    errno = 0;
    ping_pid = strtol(argv[1], &endptr, 10);

    if (0 != errno || '\0' != *endptr || ping_pid <= 0)
    {
        exit(1);
    }

    sa.sa_handler = SignalHandlerIMP;

    if (-1 == sigaction(SIGUSR2, &sa, NULL))
    {
        exit(1);
    }

    printf("pong pid: %d\n", getpid());

    if (-1 == kill((pid_t)ping_pid, SIGUSR1))
    {
        exit(1);
    }

    printf("pong -> SIGUSR1 to ping (%ld)\n", ping_pid);

    while (1)
    {
        pause();

        if (was_signal_receive)
        {
            was_signal_receive = 0;

            if (-1 == kill((pid_t)ping_pid, SIGUSR1))
            {
                exit(1);
            }

            printf("pong -> SIGUSR1 to ping (%ld)\n", ping_pid);
        }
    }

    return 0;
}

static void SignalHandlerIMP(int sig_)
{
    (void)sig_;
    was_signal_receive = 1;
}
