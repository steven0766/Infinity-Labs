#define _POSIX_C_SOURCE 200809L 
 
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
 
#include "utils.h"

static void SignalHandlerIMP(int sig_);
 
static volatile int was_signal_receive = 1;

int main()
{
	/* register handler */
	struct sigaction sa = {0};
	sa.sa_handler = SignalHandlerIMP;
	
	/* handle failure */
	if (-1 == sigaction(SIGUSR1, &sa, NULL))
	{
		exit(1);
	}
	
	/* while true */
	while (1)
	{
		/* if was_signal_receive on */
		if (was_signal_receive)
		{
			was_signal_receive = 0;
			
			/* send SIGUSR1 */
			if (-1 == kill(getppid(), SIGUSR2))
			{
				exit(1);
			}
			
			printf("child pong\n");
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
