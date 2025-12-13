/***********************************************************************
*File_name: vector.c
*Reviewer: shir
*Version:  1.0
*Date:	   12.12.25
************************************************************************/

#include <stddef.h> /* size_t       */
#include <assert.h> /* assert       */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy       */

#include "../include/vector.h"

#define VECTOR_GROWTH         2
  

size_t VectorCapacity(const vector_ty* vec);
size_t VectorSize(const vector_ty* vec);


struct vector
{
    char* head;
    char* end;
    size_t element_size;
    char* start;
};


vector_ty* VectorCreate(size_t capacity, size_t element_size)
{
    vector_ty* vector = (vector_ty*)malloc(sizeof(vector_ty));
    
    if (NULL == vector)
    {
        return NULL;
    }
    
    vector -> start = (char*) malloc(capacity * element_size);
    
    if (NULL == vector -> start)
    {
        free(vector);
        return NULL;
    }
    
    vector -> head = vector -> start;
    vector -> end = vector -> start + (capacity * element_size);
    vector -> element_size = element_size;
    
    return vector;
}

void VectorDestroy(vector_ty* vec)
{
    free(vec -> start);
    free(vec);
    vec = NULL;
}

int VectorPushBack(vector_ty* vector, void* data)
{
    size_t size = 0;
    size_t capacity = 0;
    char* new_start = NULL;
    size_t new_capacity = 0;

    assert(vector);
    assert(data);
    
    size = VectorSize(vector);
    capacity = VectorCapacity(vector);
    
    if (capacity == size)
    {
        new_capacity = (0 == capacity) ? 1 : capacity * VECTOR_GROWTH;

        new_start = (char*)realloc(vector -> start,
                                    new_capacity * vector -> element_size);
                                    
        if (NULL == new_start)
        {
            return 1;
        }
        
        vector -> start = new_start;
        vector -> head  = new_start + (size * vector -> element_size);
        vector -> end   = new_start + (new_capacity * vector -> element_size);
    }
    
    memcpy(vector -> head, data, vector -> element_size);
    vector -> head += vector -> element_size;
    
    return 0;
}

void VectorPopBack(vector_ty* vector)
{
    assert(vector);
    assert(vector -> start != vector -> head);
    
    vector -> head -= vector -> element_size;
}

void* VectorGetAccessToElement(const vector_ty* vec, size_t idx)
{
    assert(vec);
    
    return (void*)((vec -> start) + (vec -> element_size * idx));
}

size_t VectorSize(const vector_ty* vec)
{
    assert(vec);
    
    return ((vec -> head - vec -> start) / vec -> element_size);
}

size_t VectorCapacity(const vector_ty* vec)
{
    assert(vec);
    
    return ((vec -> end - vec -> start) / vec -> element_size);
}

int VectorShrinkToFit(vector_ty* vector)
{
    size_t size = 0;
    char* new_start = NULL;
    
    assert(vector);
    
    size = VectorSize(vector);
    
    if (size == VectorCapacity(vector))
    {
        return 0;
    }
    
    new_start = (char*)realloc(vector -> start, size * vector -> element_size);
    
    if (NULL == new_start)
    {
        return 1;
    }
    
    vector -> start = new_start;
        vector -> head  = new_start + (size * vector -> element_size);
    vector -> end   = new_start + (size * vector -> element_size);
    
    return  0;
}

int VectorReserve(vector_ty* vector, size_t new_capacity)
{
    size_t size = 0;
    size_t capacity = 0;
    char* new_start = NULL;

    assert(vector);

    size = VectorSize(vector);
    capacity = VectorCapacity(vector);
    
    assert(new_capacity >= size);
    
    if (new_capacity <= capacity)
    {
        return 0;
    }

    new_start = (char*)realloc(vector->start,
                               new_capacity * vector->element_size);
    if (NULL == new_start)
    {
        return 1;
    }

    vector->start = new_start;
    vector->head  = new_start + (size * vector->element_size);
    vector->end   = new_start + (new_capacity * vector->element_size);
    
    return 0;
}


