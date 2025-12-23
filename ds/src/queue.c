/***********************************************************************
*File_name: queue.c
*Reviewer: itai
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <assert.h>    /* assert       */
#include <stdlib.h>    /* malloc, free */

#include "queue.h"
#include "slist.h"


struct queue
{
    slist_ty* list;
};

queue_ty* QueueCreate(void)
{
    queue_ty* queue = (queue_ty*)malloc(sizeof(queue_ty));
    
    if (NULL == queue)
    {
        return NULL;
    }
    
    queue->list = SListCreate();
    if (NULL == queue->list)
    {
        free(queue);
        return NULL;
    }
    
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
    
    assert(NULL != queue_p);
    iter = SlistInsertBefore(queue_p->list, SListEndIter(queue_p->list), data);
    return (NULL == iter);
}

void QueueDequeue(queue_ty* queue_p)
{
    assert(NULL != queue_p);
    assert(1 != QueueIsEmpty(queue_p));
    SListRemove(SListBeginIter(queue_p->list));
}

void* QueuePeek(const queue_ty* queue_p)
{
    assert(NULL != queue_p);
    return IterGetData(SListBeginIter(queue_p->list));
}

int QueueIsEmpty(const queue_ty* queue_p)
{
    assert(NULL != queue_p);
    return (SListBeginIter(queue_p->list) == SListEndIter(queue_p->list)); 
}

size_t QueueSize(const queue_ty* queue_p)
{
    assert(NULL != queue_p);
    return SListCount(queue_p->list);
}

void QueueAppend(queue_ty* queue_p1, queue_ty* queue_p2)
{
    assert(NULL != queue_p1);
    assert(NULL != queue_p2);
    SListAppend(queue_p1->list, queue_p2->list);
}
