#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free */
#include <stddef.h>     /* size_t */
#include <assert.h>     /* assert */
#include <pthread.h>    /* pthread_t, pthread_create, pthread_join, pthread_mutex_t */
#include <semaphore.h>  /* sem_t, sem_init, sem_wait, sem_post, sem_destroy */

#include "utils.h"
#include "queue.h"

enum
{
	NUM_PRODUCERS = 3,
	NUM_CONSUMERS = 3,
	NUM_PRODUCTS = 20,
	QUEUE_CAPACITY = 10,
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

static void EnqueueProductIMP(int value_);
static int DequeueProductIMP(void);

static queue_ty* g_queue = NULL;
static pthread_mutex_t g_queue_mutex;
static sem_t g_empty_sem;
static sem_t g_full_sem;

int main(void)
{
	pthread_t producers[NUM_PRODUCERS] = {0};
	pthread_t consumers[NUM_CONSUMERS] = {0};
	thread_info_t producer_info[NUM_PRODUCERS] = {0};
	thread_info_t consumer_info[NUM_CONSUMERS] = {0};
	size_t i = 0;
	int status = 0;

	/* init shared queue */
	g_queue = QueueCreate();
	EXIT_IF_BAD(NULL != g_queue, 1, "QueueCreate failed");

	/* init mutex */
	status = pthread_mutex_init(&g_queue_mutex, NULL);
	EXIT_IF_BAD(0 == status, 1, "pthread_mutex_init failed");

	/* init empty semaphore to QUEUE_CAPACITY */
	status = sem_init(&g_empty_sem, 0, QUEUE_CAPACITY);
	EXIT_IF_BAD(0 == status, 1, "sem_init empty failed");

	/* init full semaphore to 0 */
	status = sem_init(&g_full_sem, 0, 0);
	EXIT_IF_BAD(0 == status, 1, "sem_init full failed");

	/* create producers in loop */
	while (i < NUM_PRODUCERS)
	{
		producer_info[i].id = (int)i + 1;

		status = pthread_create(&producers[i], NULL, ProducerThreadIMP, &producer_info[i]);
		EXIT_IF_BAD(0 == status, 1, "pthread_create producer failed");

		++i;
	}

	i = 0;

	/* create consumers in loop */
	while (i < NUM_CONSUMERS)
	{
		consumer_info[i].id = (int)i + 1;

		status = pthread_create(&consumers[i], NULL, ConsumerThreadIMP, &consumer_info[i]);
		EXIT_IF_BAD(0 == status, 1, "pthread_create consumer failed");

		++i;
	}

	i = 0;

	/* join producers */
	while (i < NUM_PRODUCERS)
	{
		status = pthread_join(producers[i], NULL);
		EXIT_IF_BAD(0 == status, 1, "pthread_join producer failed");

		++i;
	}

	i = 0;

	/* insert poison pills for consumers */
	while (i < NUM_CONSUMERS)
	{
		sem_wait(&g_empty_sem);

		pthread_mutex_lock(&g_queue_mutex);
		EnqueueProductIMP(POISON_PILL);
		pthread_mutex_unlock(&g_queue_mutex);

		sem_post(&g_full_sem);

		++i;
	}

	i = 0;

	/* join consumers */
	while (i < NUM_CONSUMERS)
	{
		status = pthread_join(consumers[i], NULL);
		EXIT_IF_BAD(0 == status, 1, "pthread_join consumer failed");

		++i;
	}

	/* cleanup */
	status = pthread_mutex_destroy(&g_queue_mutex);
	EXIT_IF_BAD(0 == status, 1, "pthread_mutex_destroy failed");

	status = sem_destroy(&g_empty_sem);
	EXIT_IF_BAD(0 == status, 1, "sem_destroy empty failed");

	status = sem_destroy(&g_full_sem);
	EXIT_IF_BAD(0 == status, 1, "sem_destroy full failed");

	QueueDestroy(g_queue);
	g_queue = NULL;

	return 0;
}

static void* ProducerThreadIMP(void* arg_)
{
	thread_info_t* info = (thread_info_t*)arg_;
	size_t i = 0;
	int value = 0;

	assert(NULL != info);

	/* loop */
	while (i < NUM_PRODUCTS)
	{
		/* value = ProduceIMP() */
		value = ProduceIMP(info->id, (int)i);

		/* sem_wait empty */
		sem_wait(&g_empty_sem);

		/* lock mutex */
		pthread_mutex_lock(&g_queue_mutex);

		/* enqueue value into queue */
		EnqueueProductIMP(value);

		/* unlock mutex */
		pthread_mutex_unlock(&g_queue_mutex);

		/* sem_post full */
		sem_post(&g_full_sem);

		++i;
	}

	return NULL;
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
