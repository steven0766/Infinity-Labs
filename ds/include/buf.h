#ifndef __BUF_H__
#define __BUF_H__

#include <stddef.h>			/* size_t */

typedef struct buf buf_ty;

/****************************************************************************** 
*   Creates a buffer instance of "capacity" bytes.
*	Return Value: pointer to "buf" object, or NULL in case of a failure.
*	O(1)
******************************************************************************/
buf_ty* BufCreate(size_t capacity);

/****************************************************************************** 
*   Destroy "buf" instance. No operation is performed if "buf"
*		is NULL.
*   O(n)
******************************************************************************/
void BufDestroy(buf_ty* buf);

/****************************************************************************** 
*   Read "n_bytes" from "buf" into "dest". If "n_bytes" is higher than "buf"
*		size, only "size" amount of bytes will be read.
*	Return value: returns the number of bytes that were read successfully.
*	Undefined behavior if "buf" or "dest" are NULL.
*   O(n)
******************************************************************************/
size_t BufRead(buf_ty* buf, size_t n_bytes, void* dest);

/****************************************************************************** 
*   Writes "n_bytes" from "src" into "buf", until full capacity is reached.
*	Return value: returns the number of bytes that were written successfully.
*	Undefined behavior if "buf" or "src" are NULL.
*   O(n)
******************************************************************************/
size_t BufWrite(buf_ty* buf, size_t n_bytes, const void* src);

/******************************************************************************
* 	Returns the number of bytes available "buf".
* 	Undefined behaviour if "buf" is NULL.
*   O(1)
******************************************************************************/
size_t BufFreeSpace(const buf_ty* buf);

/******************************************************************************
* 	Returns "buf" capacity.
* 	Undefined behaviour if "buf" is NULL.
*   O(1)
******************************************************************************/
size_t BufCap(const buf_ty* buf);

/******************************************************************************
* 	Is "buf" Empty?
* 	Undefined behaviour if "buf" is NULL.
*   O(1)
******************************************************************************/
int BufIsEmpty(const buf_ty* buf);

#endif /* __BUF_H__ */
