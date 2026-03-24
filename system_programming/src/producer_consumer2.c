

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
