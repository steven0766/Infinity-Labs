#define _POSIX_C_SOURCE 200809L

#include <stdio.h>      /* printf */
#include <signal.h>     /* sigaction, siginfo_t, SIGUSR1, SIGUSR2 */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* getpid, pause, kill */
#include <stdlib.h>     /* exit */

#include "utils.h"

static void SignalHandlerIMP(int sig_, siginfo_t *info_, void *ucontext_);

static volatile sig_atomic_t was_signal_receive = 0;
static volatile sig_atomic_t pong_pid = 0;

int main()
{
    struct sigaction sa = {0};

    sa.sa_sigaction = SignalHandlerIMP;
    sa.sa_flags = SA_SIGINFO;

    if (-1 == sigaction(SIGUSR1, &sa, NULL))
    {
        exit(1);
    }

    printf("ping pid: %d\n", getpid());
    printf("waiting for pong...\n");

    while (1)
    {
        pause();

        if (was_signal_receive)
        {
            was_signal_receive = 0;

            if (0 != pong_pid)
            {
                if (-1 == kill((pid_t)pong_pid, SIGUSR2))
                {
                    exit(1);
                }

                printf("ping -> SIGUSR2 to pong (%d)\n", (int)pong_pid);
            }
        }
    }

    return 0;
}

static void SignalHandlerIMP(int sig_, siginfo_t *info_, void *ucontext_)
{
    (void)sig_;
    (void)ucontext_;

    if (NULL != info_)
    {
        pong_pid = info_->si_pid;
    }

    was_signal_receive = 1;
}
