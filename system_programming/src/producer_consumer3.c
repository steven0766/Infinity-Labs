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
		/* value = ProduceIMP(...) */
		/* lock mutex */
		/* push value into linked list */
		/* unlock mutex */
		/* sem_post */

	/* return NULL */
}
