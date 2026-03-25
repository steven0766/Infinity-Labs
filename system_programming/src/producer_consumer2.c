static void* ProducerThreadIMP(void* arg_);
static void* ConsumerThreadIMP(void* arg_);
static int ProduceIMP(int id, int i);
static void ConsumeIMP(int id, int value);

int main(void)
{
	/* init shared list */
	g_products = SListCreate();
	EXIT_IF_BAD(NULL != g_products, 1, "failed to create list");

	/* init mutex */
	pthread_mutex_init(&g_products_mutex, NULL);

	/* create producers in loop */
	while (curr_producer != end_producers)
	{
		pthread_create(curr_producer, NULL, ProducerThreadIMP, &producer_info[i]);
		++curr_producer;
		++i;
	}

	i = 0;

	/* create consumers in loop */
	while (curr_consumer != end_consumers)
	{
		pthread_create(curr_consumer, NULL, ConsumerThreadIMP, &consumer_info[i]);
		++curr_consumer;
		++i;
	}

	/* join producers */
	curr_producer = producers;
	while (curr_producer != end_producers)
	{
		pthread_join(*curr_producer, NULL);
		++curr_producer;
	}

	/* join consumers */
	curr_consumer = consumers;
	while (curr_consumer != end_consumers)
	{
		pthread_join(*curr_consumer, NULL);
		++curr_consumer;
	}

	/* cleanup */
	pthread_mutex_destroy(&g_products_mutex);

	return 0;
}

static void* ProducerThreadIMP(void* arg_)
{
	thread_info_t* info = (thread_info_t*)arg_;
	size_t i = 0;
	int* product = NULL;

	assert(NULL != info);

	/* loop NUM_PRODUCTS_PER_PRODUCER times */
	while (i < NUM_PRODUCTS)
	{
		/* value = ProduceIMP(id, i) */
		product = ProduceIMP(info->id);
		EXIT_IF_BAD(NULL != product, 1, "failed to create product");

		/* lock mutex */
		pthread_mutex_lock(&g_products_mutex);

		/* push value into linked list */
		AddProductIMP(product);

		/* unlock mutex */
		pthread_mutex_unlock(&g_products_mutex);

		++i;
	}

	return NULL;
}

static void* ConsumerThreadIMP(void* arg_)
{
	thread_info_t* info = (thread_info_t*)arg_;
	int* product = NULL;

	assert(NULL != info);

	/* loop forever */
	while (1)
	{
		/* lock mutex */
		pthread_mutex_lock(&g_products_mutex);

		/* if list is not empty */
		if (!SListIterIsEqual(SListEndIter(g_products), SListBeginIter(g_products)))
		{
			/* value = pop from linked list */
			product = RemoveProductIMP();

			/* unlock mutex */
			pthread_mutex_unlock(&g_products_mutex);

			/* ConsumeIMP(id, value) */
			ConsumeIMP(info->id, product);
		}
		/* else if producers_finished == TRUE */
		else if (TRUE == g_producers_finished)
		{
			/* unlock mutex */
			pthread_mutex_unlock(&g_products_mutex);

			/* break */
			break;
		}
		/* else */
		else
		{
			/* unlock mutex */
			pthread_mutex_unlock(&g_products_mutex);

			/* continue */
			continue;
		}
	}

	return NULL;
}

static int ProduceIMP(int id, int i)
{
	/* generate value id * 100 + i */
	int ret = id * 100 + i;

	/* print "Producer id produced value" */
	printf("thread id: %d produced: %d\n", id, ret);

	/* return value */
	return ret;
}

static void ConsumeIMP(int id, int value)
{
	/* print "Consumer id consumed value" */
	printf("thread id: %d, consumed: %d\n", id, value);
}


