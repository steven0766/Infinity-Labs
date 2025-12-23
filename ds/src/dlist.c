/***********************************************************************
*File_name: dlist.c
*Reviewer: rozaline
*Version:  1.0
*Date:	   23.12.25
************************************************************************/


#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */


#include "dlist.h"


static dlist_iter_ty NodeToIter(const dlist_node_ty* node);
static int Count(void* data, void* cnt);


struct dlist_node
{
    void* data;
    dlist_node_ty* next;
    dlist_node_ty* prev;
};

struct dlist
{
    dlist_node_ty head;
    dlist_node_ty tail;
};

dlist_ty* DListCreate(void)
{
    dlist_ty* dlist = (dlist_ty*)malloc(sizeof(dlist_ty));
    
    if (NULL == dlist)
    {
        return NULL;
    }
    
    dlist->head.next = &dlist->tail;
    dlist->head.prev = NULL;
    dlist->head.data = NULL;
    
    dlist->tail.next = NULL;
    dlist->tail.prev = &dlist->head;
    dlist->tail.data = NULL;
    
    return dlist;
}

void DListDestroy(dlist_ty* dlist)
{
    dlist_iter_ty iter1 = NULL;
    dlist_iter_ty iter2 = NULL;
    
    assert(NULL != dlist);
    
    iter1 = DListBeginIter(dlist);
    iter2 = DListEndIter(dlist);
    
    while (iter1 != iter2)
    {
        iter1 = DListRemove(iter1);
    }
    
    free(dlist);
    dlist = NULL;
}

size_t DListCount(const dlist_ty* dlist)
{
    size_t cnt = 0;
    
    assert(NULL != dlist);

    DListForEach(DListBeginIter(dlist), DListEndIter(dlist), 
    				  Count,  &cnt);
    
    return cnt;
}

int DListIsEmpty(const dlist_ty* dlist)
{
    return DListIterIsEqual(DListBeginIter(dlist), DListEndIter(dlist));
}

dlist_iter_ty DListInsertBefore(dlist_ty* dlist, 
                                dlist_iter_ty where, 
    			        void* new_data)
{
    dlist_iter_ty prev_iter = NULL;
    dlist_node_ty* node = NULL;
    
    assert(NULL != dlist);
    assert(NULL != where);
    
    node = (dlist_node_ty*)malloc(sizeof(dlist_node_ty));
    
    if (NULL == node)
    {
        return DListEndIter(dlist);
    }
    
    prev_iter = DListIterPrev(where);
    
    prev_iter->next = node;
    where->prev = node;
    node->data = new_data;
    node->next = where;
    node->prev = prev_iter;
    
    return NodeToIter(node);
}

dlist_iter_ty DListPushFront(dlist_ty* dlist, void* data)
{
    assert(NULL != dlist);
    return DListInsertBefore(dlist, DListBeginIter(dlist), data);
}

void DListPopFront(dlist_ty* dlist)
{
    assert(NULL != dlist);
    assert(DListIsEmpty(dlist));
    DListRemove(DListBeginIter(dlist));
}

dlist_iter_ty DListPushBack(dlist_ty* dlist, void* data)
{
    assert(NULL != dlist);
    return DListInsertBefore(dlist, DListEndIter(dlist), data);
}

void DListPopBack(dlist_ty* dlist)
{
    assert(NULL != dlist);
    assert(DListIsEmpty(dlist));
    DListRemove(DListEndIter(dlist)->prev);
}

dlist_iter_ty DListRemove(dlist_iter_ty iter)
{
    dlist_iter_ty prev_iter = NULL;
    dlist_iter_ty next_iter = NULL;
    
    assert(iter);
    
    prev_iter = DListIterPrev(iter);
    next_iter = DListIterNext(iter);
    
    prev_iter->next = next_iter;
    next_iter->prev = prev_iter;
    
    free(iter);
    iter = NULL;
    
    return next_iter;
}

void* DListIterGetData(const dlist_iter_ty iter)
{
    assert(NULL != iter);
    return iter->data;
}

void DListIterSetData(dlist_iter_ty iter, void* data)
{
    assert(NULL != iter);
    iter->data = data;
}

dlist_iter_ty DListEndIter(const dlist_ty* dlist)
{
    assert(NULL != dlist);
    return NodeToIter(&dlist->tail);
}

dlist_iter_ty DListBeginIter(const dlist_ty* dlist)
{
    assert(NULL != dlist);
    return NodeToIter(dlist->head.next);
}

dlist_iter_ty DListIterNext(const dlist_iter_ty iter)
{
    assert(NULL != iter);
    return iter->next;
}

dlist_iter_ty DListIterPrev(const dlist_iter_ty iter)
{
    assert(NULL != iter);
    return iter->prev;
}

int DListForEach(dlist_iter_ty from, dlist_iter_ty to, 
    				  action_func_ty action,  void *param)
{
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != action);
    
    while (from != to)
    {
        if (0 != action(DListIterGetData(from), param))
        {
            return 1;
        }
        
        from = DListIterNext(from);
    }
    
    return 0;
}

int DListIterIsEqual(const dlist_iter_ty iter1, const dlist_iter_ty iter2)
{
    return (iter1 == iter2) ? 0 : 1;
}
/**********************************************/
dlist_iter_ty DListFind(const dlist_iter_ty from, const dlist_iter_ty to, 
    			    is_match_func_ty is_match, void *param)
{
    dlist_iter_ty iter = NULL;
    
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != is_match);
    
    iter = (dlist_iter_ty)from;
    
    while (iter != to)
    {
        if (0 == is_match(DListIterGetData(from), param))
        {
            break;
        }
        
        iter = DListIterNext(iter);
    }
    
    return iter;
}

int DListMultiFind(const dlist_iter_ty from, const dlist_iter_ty to, 
    					dlist_ty* dest,
    					is_match_func_ty is_match, void* param)
{
    dlist_iter_ty iter = NULL;
    
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != dest);
    assert(NULL != is_match);
    
    iter = (dlist_iter_ty)from;
    
    while (iter != to)
    {
        if (0 == is_match(DListIterGetData(iter), param))
        {
            if (NULL == DListPushBack(dest, DListIterGetData(iter)))
            {
                return 1;
            }
        }
        
        iter = DListIterNext(iter);
    }
    
    return 0;
}

void DListSplice(dlist_iter_ty from, dlist_iter_ty to, dlist_iter_ty where)
{
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != where);
    
    from->prev->next = to->next;
    to->next->prev = from->prev;
    where->prev->next = from;
    from->prev = where->prev;
    where->prev = to;
    to->next = where;
}


static dlist_iter_ty NodeToIter(const dlist_node_ty* node)
{
    return (dlist_iter_ty)node;
}

static int Count(void* data, void* cnt)
{
    (void)data;
    ++(*(size_t*)cnt);
    return 0;
}
