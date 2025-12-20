#ifndef __QUEUE_H__
#define __QUEUE_H__


#include <stddef.h>     /* size_t */


typedef struct queue queue_ty;

/****************************************************************************** 
*   Creates an empty queue instance.
*	Return Value: pointer to "queue" object, or NULL in case of a failure.
*	O(1)
******************************************************************************/
queue_ty* QueueCreate(void);

/****************************************************************************** 
*   Remove "queue_p" instance. No operation is performed if "queue_p"
*		is NULL.
*   O(n)
******************************************************************************/
void QueueDestroy(queue_ty* queue_p);

/****************************************************************************** 
*   Insert a new element to the end of "queue_p".
*	Return value: 0 on success, 1 if failed.
*	Undefined behavior if "queue_p" is NULL.
*   O(1)
******************************************************************************/
int QueueEnqueue(queue_ty* queue_p, void* data);

/****************************************************************************** 
*   Remove the first element on "queue_p".
*	Undefined behaviour if "queue_p" is NULL or empty.
*   O(1)
******************************************************************************/
void QueueDequeue(queue_ty* queue_p);

/****************************************************************************** 
*   Extract the data from the first element on "queue_p".
*	Undefined behaviour if "queue_p" is NULL or empty.
*   O(1)
******************************************************************************/
void* QueuePeek(const queue_ty* queue_p);

/******************************************************************************
* 	Is "queue_p" empty? Returns 1 if empty and 0 if not.
* 	Undefined behaviour if "queue_p" is NULL.
*   O(1)
******************************************************************************/
int QueueIsEmpty(const queue_ty* queue_p);

/******************************************************************************
* 	Returns the amount of elements in "queue_p".
* 	Undefined behaviour if "queue_p" is NULL.
*   O(n)
******************************************************************************/
size_t QueueSize(const queue_ty* queue_p);

/******************************************************************************
* 	Appends "queue_p2" to "queue_p1", resulting in an empty "queue_p2".
* 	Undefined behaviour if "queue_p1" or "queue_p2" are NULL.
*   O(1)
******************************************************************************/
void QueueAppend(queue_ty* queue_p1, queue_ty* queue_p2);

#endif /* __QUEUE_H_ */
