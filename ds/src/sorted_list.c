/***********************************************************************
*File_name: dlist.c
*Reviewer: 
*Version:  1.0
*Date:	   23.12.25
************************************************************************/

#include <stdlib.h>     /* malloc, free */
#include <assert.h>     /* assert */

#include "sorted_list.h"

static dlist_iter_ty SIterToDIter(sorted_list_iter_ty s_iter);
static sorted_list_iter_ty DIterToSIter(dlist_iter_ty iter, const dlist_ty* list);
static dlist_ty* SIterToDList(sorted_list_iter_ty s_iter);

struct sorted_list
{
    dlist_ty* list;
    sorted_list_cmp_func_ty cmp;
    void* param;
};

sorted_list_ty* SortedListCreate(sorted_list_cmp_func_ty cmp, 
                                 void* param)
{
    sorted_list_ty* sorted_list = (sorted_list_ty*)malloc(sizeof(sorted_list_ty));
    
    if (NULL == sorted_list)
    {
        return NULL;
    }
    
    sorted_list->list = DListCreate();
    
    if (NULL == sorted_list->list)
    {
        free(sorted_list);
        sorted_list = NULL;
        return NULL;
    }
    
    sorted_list->cmp = cmp;
    sorted_list->param = param;
    
    return sorted_list;
}

void SortedListDestroy(sorted_list_ty* sorted_lst)
{
    if (NULL == sorted_lst)
    {
        return;
    }
    
    DListDestroy(sorted_lst->list);
    free(sorted_lst);
    sorted_lst = NULL;
}

size_t SortedListCount(const sorted_list_ty* sorted_lst)
{
    return DListCount(sorted_lst->list);
}

int SortedListIsEmpty(const sorted_list_ty* sorted_lst)
{
    return DListIsEmpty(sorted_lst->list);
}

sorted_list_iter_ty SortedListInsert(sorted_list_ty* lst, void* new_data)
{
    sorted_list_iter_ty iter = {0};
    sorted_list_iter_ty tail = {0};
    dlist_iter_ty result = NULL;
    
    assert(NULL != lst);
    
    iter = SortedListBeginIter(lst);
    tail = SortedListEndIter(lst);
    
    while (!SortedListIterIsEqual(iter, tail) &&
           (lst->cmp(SortedListIterGetData(iter), new_data, lst->param) < 0))
    {
        iter = SortedListIterNext(iter);
    }
    
    result = DListInsertBefore(lst->list,
                               SIterToDIter(iter),
                               new_data          );
    
    return DIterToSIter(result, lst->list);
}

sorted_list_iter_ty SortedListRemove(sorted_list_iter_ty iter)
{
    dlist_iter_ty rm_iter = NULL;
    dlist_ty* lst = NULL;
    
    assert(NULL != SIterToDIter(iter));
    
    rm_iter = SIterToDIter(iter);
    lst = SIterToDList(iter);
    rm_iter = DListRemove(rm_iter);
    
    return DIterToSIter(rm_iter, lst);
}

void* SortedListIterGetData(sorted_list_iter_ty iter)
{
    dlist_iter_ty data = NULL;
    
    assert(NULL != SIterToDIter(iter));
    
    data = SIterToDIter(iter);
    return DListIterGetData(data);
}

void SortedListIterSetData( sorted_list_ty* lst,
                            sorted_list_iter_ty iter,
                            void* new_data              )
{
    assert(NULL != lst);
    assert(NULL != SIterToDIter(iter));

    SortedListRemove(iter);

    SortedListInsert(lst, new_data);
}

sorted_list_iter_ty SortedListEndIter(const sorted_list_ty* lst)
{
    dlist_iter_ty iter = NULL;
    
    assert(NULL != lst);
    
    iter = DListEndIter(lst->list);
    return DIterToSIter(iter, lst->list);
}

sorted_list_iter_ty SortedListBeginIter(const sorted_list_ty* lst)
{
    dlist_iter_ty iter = NULL;
    
    assert(NULL != lst);
    
    iter = DListBeginIter(lst->list);
    return DIterToSIter(iter, lst->list);
}

sorted_list_iter_ty SortedListIterNext(sorted_list_iter_ty iter)
{
    dlist_iter_ty iter_next = NULL;
    dlist_ty* lst = NULL;
    
    assert(NULL != SIterToDIter(iter));
    
    iter_next = SIterToDIter(iter);
    lst = SIterToDList(iter);
    
    iter_next = DListIterNext(iter_next);
    
    return DIterToSIter(iter_next, lst);
}

sorted_list_iter_ty SortedListIterPrev(sorted_list_iter_ty iter)
{
    dlist_iter_ty d_iter = NULL;
    dlist_ty* lst = NULL;
    
    assert(NULL != SIterToDIter(iter));
    
    d_iter = SIterToDIter(iter);
    lst = SIterToDList(iter);
    d_iter = DListIterPrev(d_iter);
    
    return DIterToSIter(d_iter, lst);
}

int SortedListIterIsEqual( sorted_list_iter_ty iter1, 
			   sorted_list_iter_ty iter2  )
{
    dlist_iter_ty d_iter1 = NULL;
    dlist_iter_ty d_iter2 = NULL;
    
    assert(NULL != SIterToDIter(iter1));
    assert(NULL != SIterToDIter(iter2));
    
    d_iter1 = SIterToDIter(iter1);
    d_iter2 = SIterToDIter(iter2);
    
    return DListIterIsEqual(d_iter1, d_iter2);
}

int SortedListForEach( sorted_list_iter_ty from, sorted_list_iter_ty to, 
			  sorted_list_action_func_ty action, void* param)
{
    dlist_iter_ty from_iter = NULL;
    dlist_iter_ty to_iter = NULL;
    
    assert(NULL != SIterToDIter(from));
    assert(NULL != SIterToDIter(to));
    
    from_iter = SIterToDIter(from);
    to_iter = SIterToDIter(to);
    
    return DListForEach(from_iter, to_iter, action, param);
}

sorted_list_iter_ty SortedListFind(sorted_list_ty* lst, void* data)
{
    sorted_list_iter_ty iter = {0};
    sorted_list_iter_ty end_iter = {0};
    int cmp_result = 0;
    
    assert(NULL != lst);
    
    iter = SortedListBeginIter(lst);
    end_iter = SortedListEndIter(lst);
    
    while (!SortedListIterIsEqual(iter, end_iter))
    {
        cmp_result = lst->cmp(SortedListIterGetData(iter), data, lst->param);

        if (0 == cmp_result)
        {
            return iter;
        }

        if (0 < cmp_result)
        {
            break;
        }

        iter = SortedListIterNext(iter);
    }
    
    return end_iter;
}

sorted_list_iter_ty SortedListFindIf( sorted_list_iter_ty from, sorted_list_iter_ty to, 
		      sorted_list_is_match_func_ty is_match, void* param )
{
    assert(NULL != is_match);
    assert(NULL != SIterToDIter(from));
    assert(NULL != SIterToDIter(to));

    while (!SortedListIterIsEqual(from, to))
    {
        if (1 == is_match(SortedListIterGetData(from), param))
        {
            return from;
        }

        from = SortedListIterNext(from);
    }

    return to;
}

void SortedListMerge(sorted_list_ty* dest, sorted_list_ty* src)
{
    dlist_iter_ty dest_it = NULL;
    dlist_iter_ty dest_end = NULL;
    dlist_iter_ty src_it = NULL;
    dlist_iter_ty src_end = NULL;

    assert(NULL != dest);
    assert(NULL != src);
    assert(NULL != dest->cmp);

    dest_it = DListBeginIter(dest->list);
    dest_end = DListEndIter(dest->list);

    src_it = DListBeginIter(src->list);
    src_end = DListEndIter(src->list);

    while (!DListIterIsEqual(src_it, src_end))
    {
        if (DListIterIsEqual(dest_it, dest_end))
        {
            DListSplice(src_it, src_end, dest_end);
            break;
        }

        while (!DListIterIsEqual(dest_it, dest_end) &&
               (dest->cmp(DListIterGetData(dest_it),
                          DListIterGetData(src_it),
                          dest->param) < 0))
        {
            dest_it = DListIterNext(dest_it);
        }

        if (DListIterIsEqual(dest_it, dest_end))
        {
            DListSplice(src_it, src_end, dest_end);
            break;
        }
        else
        {
            dlist_iter_ty run_from = src_it;
            dlist_iter_ty run_to = src_it;

            while (!DListIterIsEqual(run_to, src_end) &&
                   (dest->cmp(DListIterGetData(run_to),
                              DListIterGetData(dest_it),
                              dest->param) <= 0))
            {
                run_to = DListIterNext(run_to);
            }

            DListSplice(run_from, run_to, dest_it);

            src_it = run_to;
        }
    }
}




#ifndef NDEBUG
static dlist_iter_ty SIterToDIter(sorted_list_iter_ty s_iter)
{
    return s_iter.iter;
}

static sorted_list_iter_ty DIterToSIter(dlist_iter_ty iter, 
                                        const dlist_ty* list)
{
    sorted_list_iter_ty s_iter = {0};
    s_iter.iter = iter;
    s_iter.list = (dlist_ty*)list;
    
    return s_iter;
}

static dlist_ty* SIterToDList(sorted_list_iter_ty s_iter)
{
    return s_iter.list;
}

#else
static dlist_iter_ty SIterToDIter(sorted_list_iter_ty s_iter)
{
    return s_iter;
}

static sorted_list_iter_ty DIterToSIter(dlist_iter_ty iter, 
                                        const dlist_ty* list)
{
    (void)list;
    return iter;
}

static dlist_ty* SIterToDList(sorted_list_iter_ty s_iter)
{
    (void)s_iter;
    return NULL;
}
#endif
