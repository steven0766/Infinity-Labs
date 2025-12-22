#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__


#include <stddef.h>	/* size_t */


typedef struct c_buffer buffer_ty;


buffer_ty* CircularBufferCreate(size_t capacity);
void CircularBufferDestroy(buffer_ty* buffer);
size_t CircularBufferRead(buffer_ty* c_buffer, size_t nbytes, void* dest);
size_t CircularBufferWrite(buffer_ty* c_buffer, size_t nbytes, const void* src);
size_t CircularBufferFreeSpace(const buffer_ty* buffer);
size_t CircularBufferCapacity(const buffer_ty* buffer);
int CircularBufferIsEmpty(const buffer_ty* buffer);


#endif  /* __CIRCULAR_BUFFER__ */
