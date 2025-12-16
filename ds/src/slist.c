#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/linkedlist.h"


#define SUCCESS     0
#define FAIL        1


#define ITER_TO_NODE(iter)      ((slist_node_ty*)(iter))
#define NODE_TO_ITER(node)      ((slist_iter_ty)(node))


static int CountNodes(void* data, void*  cnt);


struct slist
{
    slist_node_ty* head;
    slist_node_ty* tail;
};

struct slist_node
{
    void* data;
    slist_node_ty* next;
};


slist_iter_ty SListEndIter(slist_ty* slist_p)
{
    assert(NULL != slist_p);
    
    return NODE_TO_ITER(slist_p -> tail);
}

slist_iter_ty SListBeginIter(slist_ty* slist_p)
{
    assert(NULL != slist_p);
    
    return NODE_TO_ITER(slist_p -> head);
}

slist_iter_ty IterNext(slist_iter_ty iter)
{
    assert(NULL != iter);
    assert(NULL != iter -> next);
    
    return iter -> next;
}

int IterIsEqual(slist_iter_ty iter1, slist_iter_ty iter2)
{
    assert(NULL != iter1);
    assert(NULL != iter2);
    
    return iter1 == iter2;
}

void* IterGetData(slist_iter_ty iter)
{
    assert(NULL != iter);
    assert(IterNext(iter) != NULL);
    
    return ITER_TO_NODE(iter) -> data;
}

void IterSetData(slist_iter_ty iter, void* data)
{
    assert(NULL != iter);
    assert(IterNext(iter) != NULL);
    
    ITER_TO_NODE(iter) -> data = data;
}

slist_ty* SListCreate(void)
{
    slist_ty* list_p = (slist_ty*)malloc(sizeof(slist_ty));
    slist_node_ty* tail_p = NULL;
    
    if (NULL == list_p)
    {
        return NULL;
    }
    
    tail_p = (slist_node_ty*)malloc(sizeof(slist_node_ty));
    
    if (NULL == tail_p)
    {
        free(list_p);
        return NULL;
    }
    
    tail_p -> data = list_p;
    tail_p -> next = NULL;
    list_p -> tail = tail_p;
    list_p -> head = tail_p;
    
    return list_p;
}

void SListDestroy(slist_ty* slist_p)
{
    slist_iter_ty iter = slist_p -> head;
    slist_iter_ty iter_save = NULL;
    
    assert(NULL != slist_p);
    
    while (iter)
    {
        iter_save = IterNext(iter);
        free(iter);
        iter = iter_save;
    }
    
    free(slist_p);
}

size_t SListCount(const slist_ty* slist_p)
{
    size_t cnt = 0;
    
    assert(NULL != slist_p); 
    SListForEach(slist_p -> head, slist_p -> tail, CountNodes , &cnt);
    
    return cnt;
}

slist_iter_ty SListRemove( slist_iter_ty iter)
{
    slist_node_ty* node_to_rm = NULL;
    
    assert(NULL != iter);
    assert(NULL != IterNext(iter));
    
    node_to_rm = IterNext(iter);    
    IterSetData(iter, IterGetData(node_to_rm));
    iter -> next = IterNext(node_to_rm);    
    free(node_to_rm);
    
    return iter;
}

slist_iter_ty SlistInsertBefore(slist_ty* slist_p,
                                slist_iter_ty where_iter, 
                                void* new_data)
{
    slist_node_ty* new_node = (slist_node_ty*)malloc(sizeof(slist_node_ty));
    
    assert(NULL != slist_p);
    assert(NULL != IterGetData(where_iter));
    
    if (NULL == new_node)
    {
        return SListEndIter(slist_p);
    }
    
    new_node -> next = IterNext(where_iter);
    IterSetData(new_node, IterGetData(where_iter));
    IterSetData(where_iter, new_data);
    where_iter -> next = new_node;
    
    if (NULL == IterNext(NODE_TO_ITER(new_node)))
    {
        slist_p -> tail = new_node;
    }
    
    return where_iter;
}

slist_iter_ty SListFind(slist_iter_ty from, 
                        slist_iter_ty to, 
			match_func_ty is_match, void *param)
{
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != is_match);
    
    while (from != to)
    {
        if (1 == is_match(IterGetData(from), param))
        {
            return from;
        }
        
        from = IterNext(from);
    }
    
    return to;
}

int SListForEach( slist_iter_ty from, 
                  slist_iter_ty to, 
		  action_func_ty action,  void* param)
{
    assert(NULL != from);
    assert(NULL != to);
    
    while (from != to)
    {
        if (0 != action(IterGetData(from), param))
        {
            return FAIL;
        }
        
        from = IterNext(from);
    }
    
    return SUCCESS;
}

static int CountNodes(void* data, void*  cnt)
{
    (void)data;
    ++*(size_t*)cnt;
    
    return 0;
}
