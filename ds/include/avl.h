#ifndef __ILRD_AVL_H__
#define __ILRD_AVL_H__

#include <stddef.h>         /* size_t */

typedef enum {
    IN_ORDER = 0,
    PRE_ORDER = 1,
    POST_ORDER = 2
} traverse_ty; 

typedef struct avl avl_ty;

/****************************************************************************** 
*   	Compares "data1" and "data2", any other parameter may be
*		passed with "param".
*	Return value:
* 		0 > result if data1 comes before data2,
* 		0 < result if data2 comes before data1,
*		0 == result if data1 and data2 are equal
******************************************************************************/
typedef int (*avl_cmp_ty)(const void* data1, const void* data2, void* param); 

/****************************************************************************** 
*   	Perform action on data of avl, any other parameter may be
*		passed with "param".
*	Return 0 on success, any other value will be considered
*		failure.
******************************************************************************/
typedef int (*avl_action_func_ty)(void* data, void* param);


/****************************************************************************** 
* 	Description:
* 		Creates an empty avl. 
* 	Return value:	
* 		On success, a pointer to the created avl
* 		On failure, NULL
*   	Undefined Behaviour : 
*       	cmp_func is NULL
* 	O(1)
******************************************************************************/
avl_ty* AvlCreate(avl_cmp_ty cmp_func, void* param);

/****************************************************************************** 
* 	Description:
* 		Destoys avl tree. 
*   on null ptr does nothing.
* 	O(n)
******************************************************************************/
void AvlDestroy(avl_ty* avl);
/****************************************************************************** 
* 	Description:
* 		Removes data from the tree.
*   if data found it removes it and returns 0. (SUCCESS)
*   if data not found it does nothing and returns 1 (NOT FOUND)
* 
*   Undefined Behavior:
*       data is NULL or avl is NULL.
* 	O(Log(n))
******************************************************************************/
int AvlRemove(avl_ty* avl, void* data);

/****************************************************************************** 
* 	Description:
        Inserts new data to the tree.
    Undefined Behaviour:
        data is NULL or avl is NULL.
    Return Value:
        returns 0 (SUCCESS) or 1 (FAILURE).
* 	O(Log(n))
******************************************************************************/
int AvlInsert(avl_ty* avl, void* data);

/****************************************************************************** 
* 	Description:
        performs action for every node of the tree.
    Undefined Behaviour:
        action is null, avl is null
    Return Value:
        returns 0 (SUCCESS) or 1 (FAILURE).
* 	O(n)
******************************************************************************/
int AvlForEach(avl_ty* avl, traverse_ty traverse, avl_action_func_ty action, void* param);

/****************************************************************************** 
* 	Description:
       Calculate the amount of elements in the tree
    Undefined Behaviour:
        avl is null
    Return Value:
         Returns the amount of elements in the tree
* 	O(n)
******************************************************************************/
size_t AvlSize(const avl_ty* avl);
/****************************************************************************** 
* 	Description:
      Checks if the tree is empty or not.
    Undefined Behaviour:
        avl is null
    Return Value:
         Returns 1 (TRUE) if the tree is empty or 0 (FALSE) if it is not.
* 	O(1)
******************************************************************************/
int AvlIsEmpty(const avl_ty* avl);

/****************************************************************************** 
* 	Description:
      Finds elem to find in the tree and returns it
    Undefined Behaviour:
        avl is null,  elem_to_find is null
    Return Value:
        the element found or NULL if it not found.
* 	O(Log(n))
******************************************************************************/
void* AvlFind(avl_ty* avl, void* elem_to_find);

/****************************************************************************** 
* 	Description:
      Calculates height of the tree
    Undefined Behaviour:
        avl is NULL
    Return Value:
        returns the Height of the tree.
* 	O(1)
******************************************************************************/
size_t AvlHeight(const avl_ty* avl);  

#endif /* __ILRD_AVL_H__ */


