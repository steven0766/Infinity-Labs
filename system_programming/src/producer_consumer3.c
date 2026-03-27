int main(void)
{
	/* init shared list */

	/* init mutex */

	/* init semaphore to 0 */

	/* create producers in loop */

	/* create consumers in loop */

	/* join producers */

	/* join consumers */

	/* cleanup */
}

static void* ProducerThreadIMP(void* arg_)
{
	/* loop */
		/* value = ProduceIMP() */
		/* lock mutex */
		/* push value into linked list */
		/* unlock mutex */
		/* sem_post */

	/* return NULL */
}

static void* ConsumerThreadIMP(void* arg_)
{
	/* loop */
		/* sem_wait */
		/* lock mutex */
		/* value = pop from linked list */
		/* unlock mutex */
		/* ConsumeIMP() */

	/* return NULL */
}

static int ProduceIMP(int id, int i)
{
	/* generate product value */
	int ret = id * 100 + i;

	/* print generated value */
	printf("Producer %d produced %d\n", id, ret);

	/* return generated value */
	return ret;
}

static void ConsumeIMP(int id, int value)
{
	/* print consumer id and value */
	printf("Consumer %d consumed %d\n", id, value);
}
