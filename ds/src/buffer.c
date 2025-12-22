/***********************************************************************
*File_name: queue.c
*Reviewer: 
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <stdlib.h>   
#include <assert.h>   /* assert */
#include <string.h>   /* memcpy */

#include "buffer.h"


#define MIN(a,b)      (((a)>(b))?(b):(a))


size_t CircularBufferFreeSpace(const buffer_ty* buffer);


struct c_buffer
{
    size_t capacity;
    size_t front;
    size_t size;
    char data[1];
};


buffer_ty* CircularBufferCreate(size_t capacity)
{
    buffer_ty* buffer = (buffer_ty*)malloc(offsetof(buffer_ty, data) + capacity);
    
    if (NULL == buffer)
    {
        return NULL;
    }
    
    buffer->capacity = capacity;
    buffer->front = 0;
    buffer->size = 0;
    
    return buffer;
}

void CircularBufferDestroy(buffer_ty* buffer)
{
    free(buffer);
}

size_t CircularBufferRead(buffer_ty* buffer, size_t nbytes, void* dest)
{
    size_t to_read = MIN(nbytes, buffer->size);
    size_t first_chunk = MIN(to_read, buffer->capacity - buffer->front);

    assert(buffer);
    assert(dest);

    memcpy(dest, buffer->data + buffer->front, first_chunk);

    memcpy((char*)dest + first_chunk,
           buffer->data,
           to_read - first_chunk);

    buffer->front = (buffer->front + to_read) % buffer->capacity;
    buffer->size -= to_read;

    return to_read;
}


size_t CircularBufferWrite(buffer_ty* buffer, size_t nbytes, const void* src)
{
    size_t free_space = CircularBufferFreeSpace(buffer);
    size_t to_write = MIN(nbytes, free_space);
    size_t write_idx = (buffer->front + buffer->size) % buffer->capacity;
    size_t first_chunk = MIN(to_write, buffer->capacity - write_idx);

    assert(buffer);
    assert(src);

    memcpy(buffer->data + write_idx, src, first_chunk);

    memcpy(buffer->data, (char*)src + first_chunk,
           to_write - first_chunk);

    buffer->size += to_write;

    return to_write;
}


size_t CircularBufferFreeSpace(const buffer_ty* buffer)
{
    return (buffer->capacity - buffer->size);
}

size_t CircularBufferCapacity(const buffer_ty* buffer)
{
    return buffer->capacity;
}

int CircularBufferIsEmpty(const buffer_ty* buffer)
{
    return (0 == buffer->size);
}

