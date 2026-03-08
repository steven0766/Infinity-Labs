static int was_signal_receive = 0;

int main()
{
	/* register handler */
	/* fork */
	/* if pid equal -1 */
		/* handle failure */
	/* if pid is child */
		/* toggle was_signal_receive on */
	
	/* while true */
		/* if was_signal_receive on */
			/* if I am child */
				/* send SIGURS2 */
				/* toggle was_signal_receive off */
			/* if I am parent */
				/* send SIGURS1 */
				/* toggle was_signal_receive off */
}

static void SignalHandlerIMP(int sig_)
{
	/* (void)sig_ */
	/* toggle was_signal_receive on */
}
