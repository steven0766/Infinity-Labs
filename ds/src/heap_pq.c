pq_ty* PQCreate(pq_cmp_ty cmp, void* param)
{
	/* assert */
	
	/* allocate pq struct */
	
	/* validate pq allocation */
	
	/* create vector with cap 1 */
	
	/* validate vector allocation */
	
	/* add vector and cmp to pq */
	
	/* return pq */
}

void PQDestroy(pq_ty* q)
{
	/* assert */
	
	/* destroy vector */
	
	/* free pq */
}

status_ty PQEnqueue(pq_ty* q, void* data)
{
	/* assert */
	
	/* add data to the end of vector */
	
	/* handle data didn't add to vector */
	
	/* heapify up */
	
	/* return status */
}


