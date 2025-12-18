/***********************************************************************
*File_name: queue.c
*Reviewer: itai
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "queue.h"


struct queue
{
    slist_ty* list;
};

queue_ty* QueueCreate(void)
{
    queue_ty* queue = (queue_ty*)malloc(sizeof(queue_ty));
    assert(NULL != queue);
    
    queue->list = SListCreate();
    assert(NULL != queue->list);
    
    return queue;
}

void QueueDestroy(queue_ty* queue_p)
{
    if (NULL == queue_p)
    {
        return;
    }
    
    SListDestroy(queue_p->list);
    free(queue_p);
    queue_p = NULL;
}

int QueueEnqueue(queue_ty* queue_p, void* data)
{
    slist_iter_ty iter = NULL;
    iter = SlistInsertBefore(queue_p->list, SListEndIter(queue_p->list), data);
    return (NULL == iter);
}

void QueueDequeue(queue_ty* queue_p)
{
    SListRemove(SListBeginIter(queue_p->list));
}

void* QueuePeek(const queue_ty* queue_p)
{
    return IterGetData(SListBeginIter(queue_p->list));
}

int QueueIsEmpty(const queue_ty* queue_p)
{
    return (SListBeginIter(queue_p->list) == SListEndIter(queue_p->list)); 
}

size_t QueueSize(const queue_ty* queue_p)
{
    return SListCount(queue_p->list);
}

void QueueAppend(queue_ty* queue_p1, queue_ty* queue_p2)
{
    SListAppend(queue_p1->list, queue_p2->list);
}
