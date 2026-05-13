#include <assert.h> /* assert */
#include <pthread.h> /* pthread_t, pthread_create, pthread_join, pthread_mutex_t */
#include <stddef.h> /* size_t */
#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free */

#include "slist.h"
#include "utils.h"

enum
{
  NUM_PRODUCERS = 3,
  NUM_CONSUMERS = 3,
  NUM_PRODUCTS = 50
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
static pthread_mutex_t g_products_mutex;
static int g_producers_finished = 0;
static size_t g_finished_producers = 0;

int main(void)
{
  pthread_t consumers[NUM_CONSUMERS] = {0};
  pthread_t producers[NUM_PRODUCERS] = {0};
  thread_info_t consumer_info[NUM_CONSUMERS] = {0};
  thread_info_t producer_info[NUM_PRODUCERS] = {0};
  pthread_t* curr_consumer = consumers;
  pthread_t* curr_producer = producers;
  pthread_t* end_consumers = consumers + NUM_CONSUMERS;
  pthread_t* end_producers = producers + NUM_PRODUCERS;
  size_t i = 0;
  int status = 0;

  /* init shared list */
  g_products = SListCreate();
  EXIT_IF_BAD(NULL != g_products, 1, "failed to create list");

  /* init mutex */
  status = pthread_mutex_init(&g_products_mutex, NULL);
  EXIT_IF_BAD(0 == status, 1, "pthread_mutex_init failed");

  while (i < NUM_PRODUCERS)
  {
    producer_info[i].id = (int)i + 1;
    ++i;
  }

  i = 0;

  while (i < NUM_CONSUMERS)
  {
    consumer_info[i].id = (int)i + 1;
    ++i;
  }

  i = 0;

  /* create producers in loop */
  while (curr_producer != end_producers)
  {
    status = pthread_create(curr_producer, NULL, ProducerThreadIMP,
                            &producer_info[i]);
    EXIT_IF_BAD(0 == status, 1, "pthread_create producer failed");

    ++curr_producer;
    ++i;
  }

  i = 0;

  /* create consumers in loop */
  while (curr_consumer != end_consumers)
  {
    status = pthread_create(curr_consumer, NULL, ConsumerThreadIMP,
                            &consumer_info[i]);
    EXIT_IF_BAD(0 == status, 1, "pthread_create consumer failed");

    ++curr_consumer;
    ++i;
  }

  /* join producers */
  curr_producer = producers;
  while (curr_producer != end_producers)
  {
    status = pthread_join(*curr_producer, NULL);
    EXIT_IF_BAD(0 == status, 1, "pthread_join producer failed");

    ++curr_producer;
  }

  status = pthread_mutex_lock(&g_products_mutex);
  EXIT_IF_BAD(0 == status, 1, "pthread_mutex_lock failed");

  g_producers_finished = 1;

  status = pthread_mutex_unlock(&g_products_mutex);
  EXIT_IF_BAD(0 == status, 1, "pthread_mutex_unlock failed");

  /* join consumers */
  curr_consumer = consumers;
  while (curr_consumer != end_consumers)
  {
    status = pthread_join(*curr_consumer, NULL);
    EXIT_IF_BAD(0 == status, 1, "pthread_join consumer failed");

    ++curr_consumer;
  }

  /* cleanup */
  status = pthread_mutex_destroy(&g_products_mutex);
  EXIT_IF_BAD(0 == status, 1, "pthread_mutex_destroy failed");

  return 0;
}

static void* ProducerThreadIMP(void* arg_)
{
  thread_info_t* info = (thread_info_t*)arg_;
  size_t i = 0;
  int product = 0;
  int status = 0;

  assert(NULL != info);

  /* loop NUM_PRODUCTS_PER_PRODUCER times */
  while (i < NUM_PRODUCTS)
  {
    /* value = ProduceIMP(id, i) */
    product = ProduceIMP(info->id, (int)i);

    /* lock mutex */
    status = pthread_mutex_lock(&g_products_mutex);
    EXIT_IF_BAD(0 == status, 1, "pthread_mutex_lock failed");

    /* push value into linked list */
    AddProductIMP(product);

    /* unlock mutex */
    status = pthread_mutex_unlock(&g_products_mutex);
    EXIT_IF_BAD(0 == status, 1, "pthread_mutex_unlock failed");

    ++i;
  }

  status = pthread_mutex_lock(&g_products_mutex);
  EXIT_IF_BAD(0 == status, 1, "pthread_mutex_lock failed");

  ++g_finished_producers;
  if (NUM_PRODUCERS == g_finished_producers)
  {
    g_producers_finished = 1;
  }

  status = pthread_mutex_unlock(&g_products_mutex);
  EXIT_IF_BAD(0 == status, 1, "pthread_mutex_unlock failed");

  return NULL;
}

static void* ConsumerThreadIMP(void* arg_)
{
  thread_info_t* info = (thread_info_t*)arg_;
  int product = 0;
  int status = 0;

  assert(NULL != info);

  /* loop forever */
  while (1)
  {
    /* lock mutex */
    status = pthread_mutex_lock(&g_products_mutex);
    EXIT_IF_BAD(0 == status, 1, "pthread_mutex_lock failed");

    /* if list is not empty */
    if (!IterIsEqual(SListEndIter(g_products), SListBeginIter(g_products)))
    {
      /* value = pop from linked list */
      product = RemoveProductIMP();

      /* unlock mutex */
      status = pthread_mutex_unlock(&g_products_mutex);
      EXIT_IF_BAD(0 == status, 1, "pthread_mutex_unlock failed");

      /* ConsumeIMP(id, value) */
      ConsumeIMP(info->id, product);
    }
    /* else if producers_finished == TRUE */
    else if (1 == g_producers_finished)
    {
      /* unlock mutex */
      status = pthread_mutex_unlock(&g_products_mutex);
      EXIT_IF_BAD(0 == status, 1, "pthread_mutex_unlock failed");

      /* break */
      break;
    }
    /* else */
    else
    {
      /* unlock mutex */
      status = pthread_mutex_unlock(&g_products_mutex);
      EXIT_IF_BAD(0 == status, 1, "pthread_mutex_unlock failed");

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

static void AddProductIMP(int value_)
{
  int* data = (int*)malloc(sizeof(int));
  slist_iter_ty inserted_iter = {0};

  EXIT_IF_BAD(NULL != data, 1, "malloc failed");

  *data = value_;
  inserted_iter = SlistInsertBefore(g_products, SListEndIter(g_products), data);

  EXIT_IF_BAD(!IterIsEqual(inserted_iter, SListEndIter(g_products)), 1,
              "SlistInsertBefore failed");
}

static int RemoveProductIMP(void)
{
  slist_iter_ty begin_iter = SListBeginIter(g_products);
  int* data = NULL;
  int ret = 0;

  EXIT_IF_BAD(!IterIsEqual(begin_iter, SListEndIter(g_products)), 1,
              "remove from empty list");

  data = (int*)IterGetData(begin_iter);
  ret = *data;

  free(data);
  SListRemove(begin_iter);

  return ret;
}
