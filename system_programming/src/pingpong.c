#define _POSIX_C_SOURCE 200809L 
 
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
 
#include "utils.h"

static void SignalHandlerIMP(int sig_);
 
static volatile int was_signal_receive = 0;

int main()
{
	pid_t p = 0;
	/* register handler */
	struct sigaction sa = {0};
	sa.sa_handler = SignalHandlerIMP;
	
	/* handle failure */
	if (-1 == sigaction(SIGUSR1, &sa, NULL) || -1 == sigaction(SIGUSR2, &sa, NULL))
	{
		exit(1);
	}
	
	/* fork */
	p = fork();
	
	/* if pid equal -1 */
	if (-1 == p)
	{
		/* handle failure */
		exit(1);
	}
	
	/* if pid is child */
	if (0 == p)
	{
		/* toggle was_signal_receive on */
		was_signal_receive = 1;
	}
	
	/* while true */
	while (1)
	{
		/* if was_signal_receive on */
		if (was_signal_receive)
		{
			/* toggle was_signal_receive off */
			was_signal_receive = 0;
			/* if I am child */
			if (0 == p)
			{
				/* send SIGUSR2 */
				if (-1 == kill(getppid(), SIGUSR2))
				{
					exit(1);
				}
				
				printf("child ping\n");
				
			}
			/* if I am parent */
			else
			{
				/* send SIGUSR1 */
				if (-1 == kill(p, SIGUSR1))
				{
					exit(1);
				}
				
				printf("parent pong\n");
			}
		}
	}
	
	return 0;
}

static void SignalHandlerIMP(int sig_)
{
	/* (void)sig_ */
	(void)sig_;
	/* toggle was_signal_receive on */
	was_signal_receive = 1;
}
