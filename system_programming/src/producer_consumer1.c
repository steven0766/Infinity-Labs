

int main()
{
	/* create thread for producer */
		/* handle fail */
	
	/* create thread for consumer */
		/* handle fail */
		
	/* wait till both threads finish*/
	
	/* return 0 */
}

void* Producer(void* arg)
{
	/* loop to deploy some messages */
		/* busy wait when message ready to be consumed */
		
		/* message = ProduceIMP */
		
		/* atomic op message_produced flag on */
}

void* Consumer(void* arg)
{
	/* loop to deploy some messages */
		/* busy wait when message ready to be produced */
		
		/* get produced message */
		/* consumeIMP produced message */
}

static int ProduceIMP(int value_)
{
	/* return value * 5 */
	return value_ * 5;
}

static void ConsumeIMP(int value_)
{
	/* print */
}
