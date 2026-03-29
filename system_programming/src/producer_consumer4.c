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
