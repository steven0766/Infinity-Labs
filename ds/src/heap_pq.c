#include <assert.h> 	/* assert */
#include <stdlib.h> 	/* malloc, free */

#include "vector.h" 	/* vector api */
#include "pqueue.h" 	/* api */
#include "utils.h"		/* DEBUG_ONLY, BAD_MEM */

struct pq
{
	pq_cmp_ty cmp_func;
	void* param;
	vector_ty* data;
};

static void HeapifyUpIMP(pq_ty* pq_, size_t idx_);
static void HeapifyDownIMP(pq_ty* pq_, size_t idx_);
static void** StartIMP(const pq_ty* pq_);
static size_t ParentIMP(size_t idx_);
static size_t LChildIMP(size_t idx_);
static size_t RChildIMP(size_t idx_);
static void SwapIMP(void** data1, void** data2);

pq_ty* PQCreate(pq_cmp_ty cmp_, void* param_)
{
	/* allocate pq struct */
	pq_ty* ret= (pq_ty*)malloc(sizeof(*ret));
	
	/* assert */
	assert(cmp_);
	
	/* validate pq allocation */
	if (NULL == ret)
	{
		return NULL;
	}
	/* create vector with cap 1 */
	/* validate vector allocation */
	ret->data = VectorCreate(1, sizeof(void*));
	if (NULL == ret->data)
	{
		free(ret);
		DEBUG_ONLY(ret = BAD_MEM(pq_ty*););
		return NULL;
	}
	
	/* init vector and cmp to pq */
	ret->cmp_func = cmp_;
	ret->param = param_;
	
	/* return pq */
	return ret;
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
	DEBUG_ONLY(pq_->data = BAD_MEM(vector_ty*););
	
	/* free pq */
	free(pq_);
	DEBUG_ONLY(pq_ = BAD_MEM(pq_ty*););
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
	status_ty ret = SUCCESS;
	
	/* assert */
	assert(pq_);
	
	/* add data to the end of vector */
	ret = VectorPushBack(pq_->data, &data_);
	/* handle failure */
	
	/* heapify up on new data */
	if (SUCCESS == ret)
	{
		HeapifyUpIMP(pq_, VectorSize(pq_->data));
	}
	
	/* return status */
	return ret;
}

void* PQDequeue(pq_ty* pq_)
{
	void** start = NULL;
	void* ret = NULL;
	/* assert */
	assert(pq_);

	start = StartIMP(pq_);
	/* save root data */
	ret = *(start + 1);
	
	/* copy last element to root */
	start[1] = *(start + VectorSize(pq_->data));
	
	/* remove the last element */
	VectorPopBack(pq_->data);
	
	/* heapify down the root */
	HeapifyDownIMP(pq_, 1);
	
	/* return saved data */
	return ret;
}

void* PQRemove(pq_ty* pq_, pq_is_match_ty is_match_, void* param_)
{
	void** heap = NULL;
	size_t i = 1;
	void* ret = NULL;
	/* assert */
	assert(pq_);
	assert(is_match_);
	
	heap = StartIMP(pq_);
	
	/* find element in vector */
	/* return NULL if not found */
	while (i <= VectorSize(pq_->data) && !is_match_(heap[i], param_))
	{
		++i;
	}
	if (i > VectorSize(pq_->data))
	{
		return NULL;
	}
	
	/* save data of element */
	ret = heap[i];
	
	/* copy the last element to the element we found */
	heap[i] = heap[VectorSize(pq_->data)];
	
	/* remove last element */
	VectorPopBack(pq_->data);
	
	/* heapify up or down if needed */
	
	/* if heapify up needed */
	HeapifyUpIMP(pq_, VectorSize(pq_->data));
	/* if heapify down needed */
	HeapifyDownIMP(pq_, 1);
	
	/* return saved data */
	return ret;
}

void* PQPeek(const pq_ty* pq_)
{
	void** ret = NULL;
	/* assert */
	assert(pq_);
	assert(0 != VectorSize(pq_->data));
	
	ret = StartIMP(pq_) + 1;
	/* return first element in vector */
	return *ret;
}

static void HeapifyUpIMP(pq_ty* pq_, size_t idx_)
{
	void** heap = NULL;
	/* assert */
	assert(pq_);
	
	heap = StartIMP(pq_);
	
	/* while not top and curr < parent  */
	while (1 < idx_ && pq_->cmp_func(heap[idx_],
			 heap[ParentIMP(idx_)], pq_->param) < 0)
	{
		/* swap between parent and curr data */
		SwapIMP(heap + idx_, heap + ParentIMP(idx_));
		/* curr = parent */
		idx_ = ParentIMP(idx_);
	}
}

static void HeapifyDownIMP(pq_ty* pq_, size_t idx_)
{
	void** heap = NULL;
	size_t l_child_ = LChildIMP(idx_);
	size_t r_child_ = RChildIMP(idx_);
	size_t idx_ret_ = l_child_;
	/* assert */
	assert(pq_);
	
	heap = StartIMP(pq_);
	
	/* while not leaf and child < curr  */
	while (l_child_ <= VectorSize(pq_->data))
	{
		idx_ret_ = l_child_;
		/* swap between MIN(child left, child right) and curr data */
		if (r_child_ <= VectorSize(pq_->data))
		{
			idx_ret_ = (pq_->cmp_func(heap[l_child_],
						 heap[r_child_], pq_->param) < 0)
						 				 ? l_child_ : r_child_;
		}
		
		if (pq_->cmp_func(heap[idx_ret_], heap[idx_], pq_->param) > 0)
		{
			break;
		}
		
		SwapIMP(heap + idx_ret_, heap + idx_);
		/* curr = child */
		idx_ = idx_ret_;
		l_child_ = LChildIMP(idx_);
		r_child_ = RChildIMP(idx_);
	}
}

static void** StartIMP(const pq_ty* pq_)
{
	void** ret = NULL;
	/* assert */
	assert(pq_);
	
	/* return vector address - 1 */
	ret = ((void**)VectorGetAccessToElement(pq_->data, 0)) - 1;
	
	return ret;
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

static void SwapIMP(void** data1, void** data2)
{
	void* tmp = *data1;
	*data1 = *data2;
	*data2 = tmp;
}


















