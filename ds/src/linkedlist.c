#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "../include/linkedlist.h"


static int CountNodes(void* data, void*  cnt);


struct slist
{
    slist_node_ty* head;
    slist_node_ty* dummy;
};

struct slist_node
{
    void* data;
    slist_node_ty* next;
};

#define SUCCESS     0
#define FAIL        1


slist_iter_ty SListEndIter(slist_ty* slist_p)
{
    assert(NULL != slist_p);
    
    return slist_p -> dummy;
}

slist_iter_ty SListBeginIter(slist_ty* slist_p)
{
    assert(NULL != slist_p);
    
    return slist_p -> head;
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
    assert(iter->next != NULL);
    
    return iter -> data;
}

void IterSetData(slist_iter_ty iter, void* data)
{
    assert(NULL != iter);
    assert(iter->next != NULL);
    
    iter -> data = data;
}

slist_ty* SListCreate(void)
{
    slist_ty* list_p = (slist_ty*)malloc(sizeof(slist_ty));
    slist_node_ty* dummy_p = NULL;
    
    if (NULL == list_p)
    {
        return NULL;
    }
    
    dummy_p = (slist_node_ty*)malloc(sizeof(slist_node_ty));
    
    if (NULL == dummy_p)
    {
        free(list_p);
        list_p = NULL;
        return NULL;
    }
    
    dummy_p -> data = list_p;
    dummy_p -> next = NULL;
    list_p -> dummy = dummy_p;
    list_p -> head = dummy_p;
    
    return list_p;
}

void SListDestroy(slist_ty* slist_p)
{
    slist_iter_ty iter = slist_p -> head;
    slist_iter_ty iter_save = NULL;
    
    assert(NULL != slist_p);
    
    while (iter)
    {
        iter_save = iter -> next;
        free(iter);
        iter = NULL;
        iter = iter_save;
    }
    
    free(slist_p);
    slist_p = NULL;
}

size_t SListCount(const slist_ty* slist_p)
{
    size_t cnt = 0;
    
    assert(NULL != slist_p); 
    SListForEach(slist_p -> head, slist_p -> dummy, CountNodes , &cnt);
    
    return cnt;
}

slist_iter_ty SListRemove( slist_iter_ty iter)
{
    slist_node_ty* node_to_rm = NULL;
    
    assert(NULL != iter);
    assert(NULL != iter -> next);
    
    node_to_rm = iter -> next;
    iter -> data = node_to_rm -> data;
    iter -> next = node_to_rm -> next;
    
    free(node_to_rm);
    node_to_rm = NULL;
    
    return iter;
}

slist_iter_ty SlistInsertBefore(slist_ty* slist_p,
                                slist_iter_ty where_iter, 
                                void* new_data)
{
    slist_node_ty* new_node = (slist_node_ty*)malloc(sizeof(slist_node_ty));
    
    assert(NULL != slist_p);
    assert(NULL != where_iter);
    
    if (NULL == new_node)
    {
        return SListEndIter(slist_p);
    }
    
    new_node -> next = where_iter -> next;
    new_node -> data =where_iter -> data;
    where_iter -> data = new_data;
    where_iter -> next = new_node;
    
    if (NULL == new_node -> next)
    {
        slist_p -> dummy = new_node;
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
        if (1 == is_match(from -> data, param))
        {
            return from;
        }
        
        from = from -> next;
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
        if (0 != action(from -> data, param))
        {
            return FAIL;
        }
        
        from = from -> next;
    }
    
    return SUCCESS;
}

static int CountNodes(void* data, void*  cnt)
{
    (void)data;
    ++*(size_t*)cnt;
    
    return 0;
}
