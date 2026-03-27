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
	thread_info_t* info = (thread_info_t*)arg_;
	int value = 0;

	/* loop */
	while (1)
	{
		/* sem_wait */
		sem_wait(&g_sem);

		/* lock mutex */
		pthread_mutex_lock(&g_mutex);

		/* value = pop from linked list */
		value = RemoveProductIMP();

		/* unlock mutex */
		pthread_mutex_unlock(&g_mutex);

		/* ConsumeIMP() */
		ConsumeIMP(info->id, value);
	}

	return NULL;
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

static int RemoveProductIMP(void)
{
	slist_iter_ty iter = SListBeginIter(g_products);
	int* data = NULL;
	int ret = 0;

	EXIT_IF_BAD(!IterIsEqual(iter, SListEndIter(g_products)), 1, "empty list");

	data = (int*)IterGetData(iter);
	ret = *data;

	SListRemove(iter);
	free(data);

	return ret;
}
