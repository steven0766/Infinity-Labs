#ifndef __SLIST_EX_H__
#define __SLIST_EX_H__

typedef struct node
{
	void *data;
	struct node* next;
}node_ty;

/******************************************************************************
*	Reverse the order of a given slist.
*	Undefined behavior for:
*		head is NULL.
*		head is not the start of the list.
******************************************************************************/
node_ty* Flip(node_ty* head);

/******************************************************************************
*	Tells wether a given slist has a loop.
*	Undefined behavior for NULL pointer head.
******************************************************************************/
int HasLoop(const node_ty* head);

/******************************************************************************
*	Return a pointer to a first node mutual to both slists, if any.
*	Undefined behavior for NULL pointers head_1/head_2.
******************************************************************************/
node_ty* FindIntersection(node_ty* head_1, node_ty* head_2);
 
#endif /*__SLIST_EX_H__*/
