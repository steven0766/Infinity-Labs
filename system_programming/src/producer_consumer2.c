static void* ProducerThreadIMP(void* arg_);
static void* ConsumerThreadIMP(void* arg_);
static int ProduceIMP(int id, int i);
static void ConsumeIMP(int id, int value);

int main(void)
{
	/* init shared list */
	/* init mutex */

	/* create producer args */
	/* create consumer args */

	/* create producers in loop */
	/* create consumers in loop */

	/* join producers */
	/* join consumers */

	/* cleanup */
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


