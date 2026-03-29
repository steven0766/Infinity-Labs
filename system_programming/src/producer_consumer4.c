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
	thread_info_t* info = (thread_info_t*)arg_;
	int value = 0;

	assert(NULL != info);

	/* loop */
	while (1)
	{
		/* sem_wait full */
		sem_wait(&g_full_sem);

		/* lock mutex */
		pthread_mutex_lock(&g_queue_mutex);

		/* value = dequeue value from queue */
		value = DequeueProductIMP();

		/* unlock mutex */
		pthread_mutex_unlock(&g_queue_mutex);

		/* sem_post empty */
		sem_post(&g_empty_sem);

		if (POISON_PILL == value)
		{
			break;
		}

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

static void EnqueueProductIMP(int value_)
{
	int* data = (int*)malloc(sizeof(int));
	int enqueue_status = 0;

	EXIT_IF_BAD(NULL != data, 1, "malloc failed");

	*data = value_;
	enqueue_status = QueueEnqueue(g_queue, data);
	EXIT_IF_BAD(0 == enqueue_status, 1, "QueueEnqueue failed");
}

static int DequeueProductIMP(void)
{
	int* data = NULL;
	int ret = 0;

	EXIT_IF_BAD(0 == QueueIsEmpty(g_queue), 1, "dequeue from empty queue");

	data = (int*)QueuePeek(g_queue);
	ret = *data;

	QueueDequeue(g_queue);
	free(data);

	return ret;
}
