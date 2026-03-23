#include <stdio.h> /* printf */

#include "utils.h"      /* EXIT_IF_BAD, TRUE, FALSE */

int main()
{
	/* create thread for producer */
		/* handle fail */
	
	/* create thread for consumer */
		/* handle fail */
		
	/* wait till both threads finish*/
	
	/* return 0 */
}

static void* Producer(void* arg)
{
	/* loop to deploy some messages */
		/* busy wait when message ready to be consumed */
		
		/* message = ProduceIMP */
		
		/* atomic op message_produced flag on */
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
		while (FALSE == __atomic_load_n(&g_message_produced, __ATOMIC_SEQ_CST))
		{
			/* do nothing */
		}
		
		/* get produced message */
		value = g_message;

		/* consumeIMP produced message */
		ConsumeIMP(value);

		__atomic_store_n(&g_message_produced, FALSE, __ATOMIC_SEQ_CST);

		++i;
	}

	return NULL;
}

static int ProduceIMP(void)
{
    static int next_product = 100;

    ++next_product;
    printf("Producer: produced %d\n", next_product);

    return next_product;
}

static void ConsumeIMP(int value_)
{
	/* print */
	printf("consumed: %d", value_);
}
