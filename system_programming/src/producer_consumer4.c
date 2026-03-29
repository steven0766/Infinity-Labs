int main(void)
{
	/* init shared queue */

	/* init mutex */

	/* init empty semaphore to QUEUE_CAPACITY */

	/* init full semaphore to 0 */

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

		/* sem_wait empty */

		/* lock mutex */

		/* enqueue value into queue */

		/* unlock mutex */

		/* sem_post full */

	/* return NULL */
}

static void* ConsumerThreadIMP(void* arg_)
{
	/* loop */
		/* sem_wait full */

		/* lock mutex */

		/* value = peek queue */
		/* dequeue value from queue */

		/* unlock mutex */

		/* sem_post empty */

		/* ConsumeIMP() */

	/* return NULL */
}

static int ProduceIMP(int id, int i)
{
	/* generate product value */

	/* print generated value */

	/* return generated value */
}

static void ConsumeIMP(int id, int value)
{
	/* print consumer id and value */
}
