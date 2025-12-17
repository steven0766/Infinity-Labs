#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>	/* size_t */

typedef struct slist slist_ty;
typedef struct slist_node slist_node_ty;

/* May change - do not touch or use it */
typedef slist_node_ty  *slist_iter_ty;

typedef int (*action_func_ty) (void* data, void* param);
typedef int (*match_func_ty)(void* data, void* param);

/****************************************************************************** 
*    Returns iter to "dummy" node - invalid node
******************************************************************************/
slist_iter_ty SListEndIter(slist_ty* slist_p);

/****************************************************************************** 
*   undefined behavior if slist_p is NULL
*   in case of empty list - returns "SlistEndIter()" , dummy node 
******************************************************************************/
slist_iter_ty SListBeginIter(slist_ty* slist_p);

/******************************************************************************
*   Undefined behavior if "iter" is SlistEnd
******************************************************************************/
slist_iter_ty IterNext(slist_iter_ty iter);

/******************************************************************************
*	Returns: 1 if iter1 and iter2 are of the same node, 0 otherwise.   
*	Undefined behavior if:
*	 	"iter1"/"iter2" NULL
*		"iter1"/"iter2" not in the same list
******************************************************************************/
int IterIsEqual(slist_iter_ty iter1, slist_iter_ty iter2);

/****************************************************************************** 
*   Undefined behavior if "iter" is SlistEnd
******************************************************************************/
void* IterGetData(slist_iter_ty iter);

/****************************************************************************** 
*   Undefined behavior if "iter" is SlistEnd
******************************************************************************/
void IterSetData(slist_iter_ty iter, void* data);

/****************************************************************************** 
*   Returns NULL in case of failure
******************************************************************************/
slist_ty* SListCreate(void);

/****************************************************************************** 
*   No operations performed if slist_p is NULL
*   O(n)
******************************************************************************/
void SListDestroy(slist_ty* slist_p);

/****************************************************************************** 
*   Returning the count of nodes in slist_p (Not including dummy)
*   O(n)
******************************************************************************/
size_t SListCount(const slist_ty* slist_p);

/****************************************************************************** 
*   Undefined behaviour if iter is SlistEnd 
*   Returns iter to next node
*   O(1)
******************************************************************************/
slist_iter_ty SListRemove( slist_iter_ty iter);

/****************************************************************************** 
*   Returns "SlistEnd" in case of failure
*   O(1)
******************************************************************************/
slist_iter_ty SlistInsertBefore(slist_ty* slist_p, slist_iter_ty where_iter, 
							    void* new_data);

/****************************************************************************** 
*    [from,to)
*    In case of failure returns "to", otherwise returns iter to match node 
*    Undefinded behavior:
*        "from" or "to" are invaild iters
*        "from" and "to" don't belong to the same list
*        from > to 
*        is_match is NULL
*    O(n)
******************************************************************************/
slist_iter_ty SListFind(slist_iter_ty from, slist_iter_ty to, 
			 match_func_ty is_match, void *param);

/****************************************************************************** 
*   Stops upon first fail and returns 1 on fail, 0 on succes 
******************************************************************************/
int SListForEach( slist_iter_ty from, slist_iter_ty to, 
				  action_func_ty action,  void *param);

/******************************************************************************
* 	Appends "slist1" to "slist2", resulting in an empty "slist2".
* 	Undefined behaviour if "slist1" or "slist2" are NULL.
*   O(1)
******************************************************************************/
void SListAppend(slist_ty* slist1, slist_ty* slist2);

#endif /* __SLIST_H__ */
