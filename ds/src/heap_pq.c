#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "vector.h" /* vector api */
#include "pqueue.h" /* api */

struct pq
{
	pq_cmp_ty cmp_func;
	void* param;
	vector_ty* data;
};

pq_ty* PQCreate(pq_cmp_ty cmp_, void* param_)
{
	pq_ty* pq= NULL;
	
	/* assert */
	assert(cmp_);
	/* allocate pq struct */
	/* validate pq allocation */
	pq = (pq_ty*)malloc(sizeof(*pq));
	if (NULL == pq)
	{
		return NULL;
	}
	/* create vector with cap 1 */
	/* validate vector allocation */
	pq->data = VectorCreate(1, sizeof(void*));
	if (NULL == pq->data)
	{
		free(pq);
		pq = NULL;
		return NULL;
	}
	
	/* init vector and cmp to pq */
	pq->cmp_func = cmp_;
	pq->param = param_;
	
	/* return pq */
	return pq;
}

void PQDestroy(pq_ty* pq_)
{
	/* handle q_ is NULL */
	if (NULL == pq_)
	{
		return;
	}
	
	/* destroy vector */
	VectorDestroy(pq_->data);
	
	/* free pq */
	free(pq_);
}

size_t PQCount(const pq_ty* pq_)
{
	/* assert */
	assert(pq_);

	/* return VectorSize */
	return VectorSize(pq_->data);
}

pq_bool_ty PQIsEmpty(const pq_ty* pq_)
{
	/* assert */
	assert(pq_);
	
	/* return if VectorSize == 0 */
	return (0 == VectorSize(pq_->data)) ? PQ_TRUE : PQ_FALSE;
}

status_ty PQEnqueue(pq_ty* pq_, void* data_)
{
	/* assert */
	
	/* add data to the end of vector */
	/* handle failure */
	
	/* heapify up on new data */
	
	/* return status */
}

void* PQDequeue(pq_ty* pq_)
{
	/* assert */

	/* save root data */
	
	/* copy last element to root */
	
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
	
	/* copy the last element to the element we found */
	
	/* remove last element */
	
	/* heapify up or down if needed */
	
	/* return saved data */
}

void* PQPeek(const pq_ty* pq_)
{
	/* assert */
	
	/* return first element in vector */
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
	
		/* swap between MIN(child left, child right) that less than curr and curr data */
		
		/* curr = child */
}

static void** StartIMP(pq_ty* pq_)
{
	void** start = NULL;
	/* assert */
	assert(pq_);
	
	/* return vector address - 1 */
	start = (void**)VectorGetAccessToElement(pq_->data, 0) - 1;
	
	return start;
}

static size_t ParentIMP(size_t idx_)
{
	/* return idx / 2 */
	return idx_ / 2;
}

static size_t LChildIMP(size_t idx_)
{
	/* return idx * 2 */
	return idx_ * 2;
}

static size_t RChildIMP(size_t idx_)
{
	/* return idx * 2 + 1 */
	return idx_ * 2 + 1;
}
























