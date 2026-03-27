#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free */
#include <pthread.h>    /* pthread */
#include <semaphore.h>  /* sem_t */

#include "utils.h"
#include "slist.h"

enum
{
	NUM_PRODUCERS = 3,
	NUM_CONSUMERS = 3,
	NUM_PRODUCTS = 20,
	POISON_PILL = -1
};

typedef struct thread_info
{
	int id;
} thread_info_t;

static void* ProducerThreadIMP(void* arg_);
static void* ConsumerThreadIMP(void* arg_);
static int ProduceIMP(int id, int i);
static void ConsumeIMP(int id, int value);

static void AddProductIMP(int value_);
static int RemoveProductIMP(void);

static slist_ty* g_products = NULL;
static pthread_mutex_t g_mutex;
static sem_t g_sem;

int main(void)
{
	pthread_t producers[NUM_PRODUCERS] = {0};
	pthread_t consumers[NUM_CONSUMERS] = {0};
	thread_info_t producer_info[NUM_PRODUCERS] = {0};
	thread_info_t consumer_info[NUM_CONSUMERS] = {0};
	size_t i = 0;

	/* init shared list */
	g_products = SListCreate();
	EXIT_IF_BAD(NULL != g_products, 1, "list create failed");

	/* init mutex */
	pthread_mutex_init(&g_mutex, NULL);

	/* init semaphore to 0 */
	sem_init(&g_sem, 0, 0);

	/* create producers in loop */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		producer_info[i].id = (int)i + 1;
		pthread_create(&producers[i], NULL, ProducerThreadIMP, &producer_info[i]);
	}

	/* create consumers in loop */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		consumer_info[i].id = (int)i + 1;
		pthread_create(&consumers[i], NULL, ConsumerThreadIMP, &consumer_info[i]);
	}

	/* join producers */
	for (i = 0; i < NUM_PRODUCERS; ++i)
	{
		pthread_join(producers[i], NULL);
	}
	
	/* insert poison pills */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		pthread_mutex_lock(&g_mutex);
		AddProductIMP(POISON_PILL);
		pthread_mutex_unlock(&g_mutex);

		sem_post(&g_sem);
	}

	/* join consumers */
	for (i = 0; i < NUM_CONSUMERS; ++i)
	{
		pthread_join(consumers[i], NULL);
	}

	/* cleanup */
	pthread_mutex_destroy(&g_mutex);
	sem_destroy(&g_sem);

	return 0;
}

static void* ProducerThreadIMP(void* arg_)
{
	thread_info_t* info = (thread_info_t*)arg_;
	size_t i = 0;
	int value = 0;

	/* loop */
	while (i < NUM_PRODUCTS)
	{
		/* value = ProduceIMP() */
		value = ProduceIMP(info->id, (int)i);

		/* lock mutex */
		pthread_mutex_lock(&g_mutex);

		/* push value into linked list */
		AddProductIMP(value);

		/* unlock mutex */
		pthread_mutex_unlock(&g_mutex);

		/* sem_post */
		sem_post(&g_sem);

		++i;
	}

	return NULL;
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

static void AddProductIMP(int value_)
{
	int* data = (int*)malloc(sizeof(int));
	slist_iter_ty iter = {0};

	EXIT_IF_BAD(NULL != data, 1, "malloc failed");

	*data = value_;
	iter = SlistInsertBefore(g_products, SListEndIter(g_products), data);

	EXIT_IF_BAD(!IterIsEqual(iter, SListEndIter(g_products)), 1, "insert failed");
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
