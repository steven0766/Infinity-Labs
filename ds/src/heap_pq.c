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

void* PQRemove(pq_ty* q_, pq_is_match_ty is_match_, void* param_)
{
	/* assert */
	
	/* find element in vector */
	
	/* return NULL if not found */
	
	/* save data of element */
	
	/* swap between element and last element in vector */
	
	/* heapify up or down if needed */
	
	/* return saved data */
}

void* PQPeek(const pq_ty* q_)
{
	/* assert */
	
	/* return VectorGetAccessToElement at idx 0 */
}

static void HeapifyUpIMP(pq_ty* pq_, size_t idx_)
{
	/* assert */
	
	/* while not top and curr < parent  */
	
		/* swap between parent and curr data */
		
		/* curr = parent */
}

static void HeapifyDownIMP(pq_ty* pq_, size_t idx_)
{
	/* assert */
	
	/* while not leaf and child < curr  */
	
		/* swap between MIN(child left, child right) and curr data */
		
		/* curr = child */
}

static void* StartIMP(pq_ty* pq_)
{
	/* assert */
	
	/* return vector address - 1 */
}

static void* ParentIMP(pq_ty* pq_, size_t idx_)
{
	/* assert */
	
	/* return VectorGetAccessToElement in idx / 2 */
}

static void LChildIMP(pq_ty* pq_, size_t idx_)
{
	/* assert */
	
	/* return VectorGetAccessToElement in idx * 2 */
}

static void RChildIMP(pq_ty* pq_, size_t idx_)
{
	/* assert */
	
	/* return VectorGetAccessToElement in idx * 2 + 1 */
}
























