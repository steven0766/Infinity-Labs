#include <stdlib.h>
#include <assert.h>

#include "sorted_list.h"
#include "priority_queue.h"

struct pq
{
    sorted_list_ty* lst;
};

pq_ty* PQCreate(pq_cmp_ty cmp, void* param)
{
    pq_ty* p_q = {0};
    
    assert(NULL != cmp);
    
    p_q = (pq_ty*)malloc(sizeof(pq_ty));
    
    if (NULL == p_q)
    {
        return NULL;
    }
    
    p_q->lst = SortedListCreate(cmp, param);
    
    if (NULL == p_q)
    {
        free(p_q);
        return NULL;
    }
    
    return p_q;
}

void PQDestroy(pq_ty* q)
{
    if (NULL == q)
    {
        return;
    }
    
    SortedListDestroy(q->lst);
    free(q);
    q = NULL;
}

size_t PQCount(const pq_ty* q)
{
    assert(NULL != q);
    
    return SortedListCount(q->lst);
}

bool_ty PQIsEmpty(const pq_ty* q)
{
    assert(NULL != q);
    
    return SortedListIsEmpty(q->lst);
}

status_ty PQEnqueue(pq_ty* q, void* data)
{
    assert(NULL != q);
    
    sorted_list_iter_ty result = SortedListInsert(q->lst, data);
    return (SortedListIterIsEqual(SortedListEndIter(q->lst), result)) ? SUCCESS : FAIL;
}

void* PQDequeue(pq_ty* q)
{
    void* q_peek = NULL;
    
    assert(NULL != q);
    assert(!PQIsEmpty(q));
    
    q_peek = PQPeek(q);
    
    SortedListRemove(SortedListBeginIter(q->lst));
    return q_peek;
}

void* PQRemove(pq_ty* q, pq_is_match_ty is_match, void* param)
{
    sorted_list_iter_ty iter = {0};
    void* data = NULL;
    
    assert(NULL != q);
    assert(!PQIsEmpty(q));
    assert(NULL != is_match);
    
    SortedListRemove(iter);
    
    iter = SortedListFindIf(SortedListBeginIter(q->lst),
                                                SortedListEndIter(q->lst), 
					        is_match, param            );
    data = SortedListIterGetData(iter);
    
    return data;
}

void* PQPeek(const pq_ty* q)
{
    return SortedListIterGetData(SortedListBeginIter(q->lst));
}
