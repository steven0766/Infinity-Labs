

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

static void* Producer(void* arg)
{
	/* extract id from arg, list and mutex */

	/* loop NUM_PRODUCTS_PER_PRODUCER times */
		/* value = ProduceIMP(id, i) */

		/* lock mutex */

		/* push value into linked list */

		/* print "Producer id produced value" */

		/* unlock mutex */

	/* return NULL */
}

static void* Consumer(void* arg)
{
	/* extract id from arg, list and mutex */

	/* loop forever */
		/* lock mutex */

		/* if list is not empty */
			/* value = pop from linked list */
			/* unlock mutex */
			/* ConsumeIMP(id, value) */

		/* else if producers_finished == TRUE */
			/* unlock mutex */
			/* break */

		/* else */
			/* unlock mutex */
			/* continue */

	/* return NULL */
}
