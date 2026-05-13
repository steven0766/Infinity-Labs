#include <pthread.h> /* pthread_t, pthread_create, pthread_join */
#include <stdio.h>   /* printf */

#include "utils.h" /* EXIT_IF_BAD, TRUE, FALSE */

enum
{
  NUM_MESSAGES = 50
};

static void* Producer(void* arg);
static void* Consumer(void* arg);
static int ProduceIMP(void);
static void ConsumeIMP(int value_);

static int g_message = 0;
static int g_message_produced = 0;

int main()
{
  int status = 0;
  pthread_t producer_thread = 0;
  pthread_t consumer_thread = 0;

  /* create thread for producer */
  status = pthread_create(&producer_thread, NULL, Producer, NULL);
  EXIT_IF_BAD(0 == status, 1, "pthread_create producer failed");

  /* create thread for consumer */
  status = pthread_create(&consumer_thread, NULL, Consumer, NULL);
  EXIT_IF_BAD(0 == status, 1, "pthread_create consumer failed");

  /* wait till both threads finish*/
  status = pthread_join(producer_thread, NULL);
  EXIT_IF_BAD(0 == status, 1, "pthread_join producer failed");

  status = pthread_join(consumer_thread, NULL);
  EXIT_IF_BAD(0 == status, 1, "pthread_join consumer failed");

  /* return 0 */
  return 0;
}

static void* Producer(void* arg)
{
  size_t i = 0;

  (void)arg;

  /* loop to deploy some messages */
  while (i < NUM_MESSAGES)
  {
    /* busy wait when message ready to be consumed */
    while (1 == __atomic_load_n(&g_message_produced, __ATOMIC_SEQ_CST))
    {
      /* do nothing */
    }

    /* message = ProduceIMP */
    g_message = ProduceIMP();

    /* atomic op message_produced flag on */
    __atomic_store_n(&g_message_produced, 1, __ATOMIC_SEQ_CST);

    ++i;
  }

  return NULL;
}

static void* Consumer(void* arg)
{
  size_t i = 0;
  int value = 0;

  (void)arg;

  /* loop to deploy some messages */
  while (i < NUM_MESSAGES)
  {
    /* busy wait when message ready to be produced */
    while (0 == __atomic_load_n(&g_message_produced, __ATOMIC_SEQ_CST))
    {
      /* do nothing */
    }

    /* get produced message */
    value = g_message;

    /* consumeIMP produced message */
    ConsumeIMP(value);

    __atomic_store_n(&g_message_produced, 0, __ATOMIC_SEQ_CST);

    ++i;
  }

  return NULL;
}

static int ProduceIMP(void)
{
  static int next_product = 100;

  ++next_product;
  printf("Producer: %d\n", next_product);

  return next_product;
}

static void ConsumeIMP(int value_)
{
  /* print */
  printf("consumed: %d\n", value_);
}
