#ifndef __ILRD_BST_H__
#define __ILRD_BST_H__

#include <stddef.h>         /* size_t */

typedef struct bst bst_ty;

typedef struct bst_node bst_node_ty; 

typedef bst_node_ty* bst_iter_ty; 

/****************************************************************************** 
*   	Function compares "data1" and "data2", any other parameter may be
*		passed with "param".
*	Return value:
* 		0 > result if data1 comes before data2,
* 		0 < result if data2 comes before data1,
*		0 == result if data1 and data2 are equal
******************************************************************************/
typedef int (*bst_cmp_ty)(const void* data1, const void* data2, void* param); 

/****************************************************************************** 
*   	Function perform action on data of bst, any other parameter may be
*		passed with "param".
*	Function should return 0 on success, any other value will be considered
*		failure.
******************************************************************************/
typedef int (*bst_action_func_ty)(void* data, void* param);


/****************************************************************************** 
* 	Description:
* 		Creates an empty bst. 
* 	Return value:	
* 		On success, a pointer to the created bst
* 		On failure, NULL
*   	Undefined Behaviour : 
*       	cmp_func is NULL
* 	O(1)
******************************************************************************/
bst_ty* BstCreate(bst_cmp_ty cmp_func, void* param);


/****************************************************************************** 
* 	Description:
* 		Destroys the bst
* 		bst can be NULL
* 	Return value:
* 		None
*   	O(n)
******************************************************************************/
void BstDestroy(bst_ty* bst);

/****************************************************************************** 
* 	Description:
* 		Insert data to the bst
* 	Return value:	
* 		On success, iterator to the inserted data.
* 		On failure, "BstEnd"
* 	Undefined behaviour:
* 		bst is NULL
* 		the data is alredy in the bst
*   	AVG 	O(log n)
*	WORST 	O(n)
******************************************************************************/
bst_iter_ty BstInsert(bst_ty* bst, void* data);

/****************************************************************************** 
*  	Description:
* 		Searches for data matching the passed value
		(bst_cmp_ty returns 0)
*   	Return value:
* 		On success, returns iterator to the matching data
* 		On failure, returns "BstEnd"
*   	Undefinded behavior:
*       	bst is NULL
*   	AVG 	O(log n)
*	WORST 	O(n)
******************************************************************************/
bst_iter_ty BstFind(const bst_ty* bst, void* data);

/****************************************************************************** 
*   	Description:
* 		Removes data at iter from the bst
* 	Return value: 
* 		iterator to the next element in in-order traversal
* 	Undefined behavior: 
* 		iter is "BstEnd"
*   	O(1)
******************************************************************************/
bst_iter_ty BstRemove(bst_iter_ty iter);

/****************************************************************************** 
* 	Description:
* 		Returns the size of bst
* 	Return value: 
* 		Number of data elements in the bst
* 	Undefined behavior: 
* 		bst is NULL
*   	O(n)
******************************************************************************/
size_t BstSize(const bst_ty* bst);

/****************************************************************************** 
* 	
* 	Return value: 
* 		1 if bst is empty, 0 otherwise
* 	Undefined behavior: 
* 		bst is NULL
*   	O(1)
******************************************************************************/
int BstIsEmpty(const bst_ty* bst);

/****************************************************************************** 
* 	Description:
* 		Performs the action on each element in range [from, to) in 
*		in-order traversal 
* 	Return Value: 
* 		0 on succes, "action" return value on failture.
* 	Undefined behaviour:
* 		action is NULL 
* 		"from" or "to" are not from the bst
* 		"from" > "to"
* 	O(n)
******************************************************************************/
int BstForEach(bst_iter_ty from,
               bst_iter_ty to,
               bst_action_func_ty action,
               void* param);
               
/****************************************************************************** 
*   	Description:
*		Returns iter to the smallest data in the bst 
		(according to bst_cmp_ty)	
* 	Return value: 
* 		iter to the first data (in-order) in the bst
* 		In case of empty bst - returns "BstEndIter"
* 	Undefined behavior:
* 		bst is NULL
*   	AVG 	O(log n)
*	WORST 	O(n)
******************************************************************************/
bst_iter_ty BstItBegin(const bst_ty* bst);

/******************************************************************************
*	Description:
* 		Returns iter to next data (in-order)
* 	Return value: 
* 		iterator to the next data 
*	Undefined behavior:
* 		iter is "BstEndIter"
*	BEST	O(1)
*   	AVG 	O(log n)
*	WORST 	O(n)
******************************************************************************/
bst_iter_ty BstItNext(bst_iter_ty iter);

/******************************************************************************
*	Description:
* 		Returns iter to prev data (in-order)
* 	Return value: 
* 		iterator to the prev data
*	Undefined behavior:
*       	iter is "BstBeginIter"
*	BEST	O(1)
*   	AVG 	O(log n)
*	WORST 	O(n)
******************************************************************************/
bst_iter_ty BstItPrev(bst_iter_ty iter);

/****************************************************************************** 
*	Description:
* 		Returns iterator marking the end of the in-order bst iteration
* 	Return value: 
* 		iterator to the end of the bst 
* 	Undefined behavior:
* 		bst is NULL
* 	O(1)
******************************************************************************/
bst_iter_ty BstItEnd(const bst_ty* bst); 
  /******************************************************************************
*   	Description:
*   		Checks if iter1 equal to iter2 
* 	Return value: 
*       	Returns 1 if iter1 and iter2 refer to the same element
*       	0 otherwise
* 	Undefined behavior: 
*       	"iter1"/"iter2" not in the same bst
*   	O(1)
******************************************************************************/
int  BstItEqual(bst_iter_ty iter1,bst_iter_ty iter2);

/******************************************************************************
*   	Description:
* 		Returns data at iter
* 	Return value: 
* 		data at iter
* 	Undefined behavior: 
* 		iter is "BstEnd"
*   	O(1)
******************************************************************************/
void* BstGetData(bst_iter_ty iter);


#endif /* __ILRD_BST_H__ */

