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
static dlist_node_ty* IterToNode(const dlist_iter_ty iter);
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
    
    if (NULL == dlist)
    {
        return;
    }
    
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
    dlist_node_ty* new_node = NULL;
    dlist_node_ty* where_node = NULL;
    
    assert(NULL != dlist);
    assert(NULL != where);
    
    new_node = (dlist_node_ty*)malloc(sizeof(dlist_node_ty));
    
    if (NULL == new_node)
    {
        return DListEndIter(dlist);
    }
    
    where_node = IterToNode(where);

    new_node->data = new_data;
    new_node->prev = where_node->prev;
    new_node->next = where_node;

    where_node->prev->next = new_node;
    where_node->prev = new_node;

    return NodeToIter(new_node);
}

dlist_iter_ty DListPushFront(dlist_ty* dlist, void* data)
{
    assert(NULL != dlist);
    return DListInsertBefore(dlist, DListBeginIter(dlist), data);
}

void DListPopFront(dlist_ty* dlist)
{
    assert(NULL != dlist);
    assert(!DListIsEmpty(dlist));
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
    assert(!DListIsEmpty(dlist));
    DListRemove(DListIterPrev(DListEndIter(dlist)));
}

dlist_iter_ty DListRemove(dlist_iter_ty iter)
{
    dlist_node_ty* node = NULL;
    dlist_node_ty* next = NULL;

    assert(NULL != iter);

    node = IterToNode(iter);
    next = node->next;

    node->prev->next = next;
    next->prev = node->prev;

    free(node);

    return NodeToIter(next);
}

void* DListIterGetData(const dlist_iter_ty iter)
{
    dlist_node_ty* node = NULL;
    assert(NULL != iter);
    node = IterToNode(iter);
    return node->data;
}

void DListIterSetData(dlist_iter_ty iter, void* data)
{
    dlist_node_ty* node = NULL;
    assert(NULL != iter);
    node = IterToNode(iter);
    node->data = data;
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
    dlist_node_ty* node = NULL;
    assert(NULL != iter);
    node = IterToNode(iter);
    assert(NULL != node->next);
    return NodeToIter(node->next);
}

dlist_iter_ty DListIterPrev(const dlist_iter_ty iter)
{
    dlist_node_ty* node = NULL;
    assert(NULL != iter);
    node = IterToNode(iter);
    assert(NULL != node->prev);
    return NodeToIter(node->prev);
}

int DListForEach(dlist_iter_ty from, dlist_iter_ty to, 
    				  action_func_ty action,  void *param)
{
    int status = 0;
    
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != action);
    
    while (from != to)
    {
        status = action(DListIterGetData(from), param);
        if (0 != status)
        {
            return status;
        }
        
        from = DListIterNext(from);
    }
    
    return 0;
}

int DListIterIsEqual(const dlist_iter_ty iter1, const dlist_iter_ty iter2)
{
    dlist_node_ty* node1 = NULL;
    dlist_node_ty* node2 = NULL;
    
    assert(NULL != iter1);
    assert(NULL != iter2);
    
    node1 = IterToNode(iter1);
    node2 = IterToNode(iter2);
    return (node1 == node2) ? 1 : 0;
}
/**********************************************/
dlist_iter_ty DListFind(const dlist_iter_ty from, const dlist_iter_ty to, 
    			    is_match_func_ty is_match, void *param)
{
    dlist_iter_ty iter = NULL;

    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != is_match);

    iter = from;
    while (!DListIterIsEqual(iter, to))
    {
        if (0 == is_match(DListIterGetData(iter), param))
        {
            return iter;
        }

        iter = DListIterNext(iter);
    }

    return to;
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

    iter = from;
    while (!DListIterIsEqual(iter, to))
    {
        if (0 == is_match(DListIterGetData(iter), param))
        {
            if (DListEndIter(dest) ==
                DListPushBack(dest, DListIterGetData(iter)))
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
    dlist_node_ty* node_from = NULL;
    dlist_node_ty* node_to = NULL;
    dlist_node_ty* node_where = NULL;

    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != where);
    
    node_from = IterToNode(from);
    node_to = IterToNode(DListIterPrev(to));
    node_where = IterToNode(where);
    
    node_from->prev->next = node_to->next;
    node_to->next->prev = node_from->prev;

    node_from->prev = node_where->prev;
    node_to->next = node_where;

    node_where->prev->next = node_from;
    node_where->prev = node_to;
}


static dlist_iter_ty NodeToIter(const dlist_node_ty* node)
{
    return (dlist_iter_ty)node;
}

static dlist_node_ty* IterToNode(const dlist_iter_ty iter)
{
    return (dlist_node_ty*)iter;
}

static int Count(void* data, void* cnt)
{
    (void)data;
    ++(*(size_t*)cnt);
    return 0;
}
