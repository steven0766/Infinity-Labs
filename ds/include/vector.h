#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>	/* size_t */

typedef struct vector vector_ty;

/******************************************************************************
* Create a new vector instance, each element of "element_size" size.
* Return Value: a pointer to created vector on success, NULL otherwise.
* O(1)
******************************************************************************/
vector_ty* VectorCreate(size_t capacity, size_t element_size);

/******************************************************************************  
* Remove all "vec" elements.
* If "vec" is NULL, no operations will be performed.
* O(1)
******************************************************************************/
void VectorDestroy(vector_ty* vec);

/******************************************************************************
* Add an element to the end of the "vec". If "vec" reached full capacity,
*   "vec" is resized to fit.
* Return Value: 0 on success, 1 on failure.
* Undefined behavior if "vec" or "data" is NULL.
* O(1) amortized.
******************************************************************************/
int VectorPushBack(vector_ty* vec, void* data);

/******************************************************************************
* Remove last element from "vec".
* Undefined behavior if "vec" is NULL or empty.
* O(1)
******************************************************************************/
void VectorPopBack(vector_ty* vec);

/******************************************************************************
* Peek at the last element of "vec".
* Undefined behavior:
*   (1) If "vec" is NULL or empty.
*   (2) If idx is higher than capacity.
* O(1)
******************************************************************************/
void* VectorGetAccessToElement(const vector_ty* vec, size_t idx);

/******************************************************************************
* Returns the amount of elements occupied in "vec".
* Undefined behavior if "vec" is NULL or empty.
* O(1)
******************************************************************************/
size_t VectorSize(const vector_ty* vec);

/******************************************************************************
* Returns maximum number of elements in the "vec".
* Undefined behavior if "vec" is NULL or empty.
* O(1)
******************************************************************************/
size_t VectorCapacity(const vector_ty* vec);

/******************************************************************************
* Shrinks "vec" to a "vec" current size, copying existing elements to the
*	edited "vec".
* Return Value: 0 on success, 1 on failure.
* Undefined behavior if "vec" is NULL or empty.
* O(n)
******************************************************************************/
int VectorShrinkToFit(vector_ty* vec);

/******************************************************************************
* Resize "vec" to a larger given capacity, copying existing elements to the
*	 new "vec".
* Return Value: 0 on success, 1 on failure.
* Undefined behavior:
*   (1) If "vec" is NULL or empty.
*   (2) If "new_capacity" is smaller than "vec" current size. 
* O(n)
******************************************************************************/
int VectorReserve(vector_ty* vec, size_t new_capacity);

#endif	/* __VECTOR_H__ */ 
