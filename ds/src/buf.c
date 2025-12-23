/***********************************************************************
*File_name: buf.c
*Reviewer: itay
*Version:  1.0
*Date:	   22.12.25
************************************************************************/

#include <stdlib.h>   /* malloc, free */
#include <assert.h>   /* assert */
#include <string.h>   /* memcpy */

#include "buf.h"


#define MIN(a,b)      (((a)>(b))?(b):(a))


struct buf
{
    size_t capacity;
    size_t front;
    size_t size;
    char data[1];
};


buf_ty* BufCreate(size_t capacity)
{
    buf_ty* buffer = (buf_ty*)malloc(offsetof(buf_ty, data) + capacity);
    
    if (NULL == buffer)
    {
        return NULL;
    }
    
    buffer->capacity = capacity;
    buffer->front = 0;
    buffer->size = 0;
    
    return buffer;
}

void BufDestroy(buf_ty* buffer)
{
    free(buffer);
    buffer = NULL;
}

size_t BufRead(buf_ty* buffer, size_t nbytes, void* dest)
{
    size_t to_read = 0;
    size_t first_chunk = 0;

    assert(buffer);
    assert(dest);
    
    to_read = MIN(nbytes, buffer->size);
    first_chunk = MIN(to_read, buffer->capacity - buffer->front);

    memcpy((char*)dest, buffer->data + buffer->front, first_chunk);

    memcpy((char*)dest + first_chunk,
           buffer->data,
           to_read - first_chunk);

    buffer->front = (buffer->front + to_read) % buffer->capacity;
    buffer->size -= to_read;

    return to_read;
}


size_t BufWrite(buf_ty* buffer, size_t nbytes, const void* src)
{
    size_t free_space = 0;
    size_t to_write = 0;
    size_t write_idx = 0;
    size_t first_chunk = 0;

    assert(buffer);
    assert(src);
    
    free_space = BufFreeSpace(buffer);
    to_write = MIN(nbytes, free_space);
    write_idx = (buffer->front + buffer->size) % buffer->capacity;
    first_chunk = MIN(to_write, buffer->capacity - write_idx);

    memcpy(buffer->data + write_idx, (char*)src, first_chunk);

    memcpy(buffer->data, (char*)src + first_chunk,
           to_write - first_chunk);

    buffer->size += to_write;

    return to_write;
}


size_t BufFreeSpace(const buf_ty* buffer)
{
    assert(NULL != buffer);
    return (buffer->capacity - buffer->size);
}

size_t BufCap(const buf_ty* buffer)
{
    assert(NULL != buffer);
    return buffer->capacity;
}

int BufIsEmpty(const buf_ty* buffer)
{
    assert(NULL != buffer);
    return (0 == buffer->size);
}

