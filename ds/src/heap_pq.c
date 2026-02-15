pq_ty* PQCreate(pq_cmp_ty cmp_, void* param_)
{
	/* assert */
	
	/* allocate pq struct */
	
	/* validate pq allocation */
	
	/* create vector with cap 1 */
	
	/* validate vector allocation */
	
	/* add vector and cmp to pq */
	
	/* return pq */
}

void PQDestroy(pq_ty* q_)
{
	/* assert */
	
	/* destroy vector */
	
	/* free pq */
}

size_t PQCount(const pq_ty* q_)
{
	/* assert */

	/* return VectorSize */
}

pq_bool_ty PQIsEmpty(const pq_ty* q_)
{
	/* assert */
	
	/* return if VectorSize == 0 */
}

status_ty PQEnqueue(pq_ty* q_, void* data_)
{
	/* assert */
	
	/* add data to the end of vector */
	
	/* handle data didn't add to vector */
	
	/* heapify up */
	
	/* return status */
}

void* PQDequeue(pq_ty* q_)
{
	/* assert */
	
	/* return NULL if pq is empty */
	
	/* save root data */
	
	/* swap between first and last element */
	
	/* remove the last element */
	
	/* heapify down the root */
	
	/* return saved data */
}

void* PQPeek(const pq_ty* q_)
{
	/* assert */
	
	/* return VectorGetAccessToElement at idx 0 */
}
