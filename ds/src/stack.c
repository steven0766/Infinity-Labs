/***********************************************************************
*File_name: stack.c
*Reviewer: Nimrod
*Version:  1.0
*Date:	   10.12.25
************************************************************************/


#include <stdlib.h>   /*   malloc, free  */
#include <stddef.h>   /*   size_t        */
#include <string.h>   /*   memcpy        */
#include <assert.h>   /*   assert        */

#include "../include/stack.h"


struct stack_s
{
    size_t element_size;
    char* end;
    char* head;
    char start[1];
};


stack_ty* StackCreate(size_t capacity, size_t ele_size)
{
    stack_ty* stk = (stack_ty*)malloc((offsetof(stack_ty, start)) + capacity * ele_size);
    if (NULL == stk)
    {
    	return NULL;
    }
    stk -> element_size = ele_size;
    stk -> end = stk -> start + capacity * ele_size;
    stk -> head = stk -> start;
    
    return stk;
}

void StackDestroy(stack_ty* stack)
{
    free(stack);
    
    stack = NULL;
}

void StackPop(stack_ty* stack)
{
    assert(stack);
    assert(stack -> head > stack -> start);
    
    stack -> head -= stack -> element_size;
}

void StackPush(stack_ty* stack, void* new_element)
{
    assert(stack);
    assert(new_element);
    assert(stack -> head + stack -> element_size <= stack -> end);
    
    memcpy(stack -> head, new_element, stack -> element_size);
    stack -> head += stack -> element_size;
}

void* StackPeek(stack_ty* stack)
{
    assert(stack);
    assert(stack->head > stack->start);
    
    return (stack -> head - stack -> element_size);
}

size_t StackSize(stack_ty* stack)
{
    assert(stack);
    return ((stack -> head - stack -> start) / stack -> element_size);
}

size_t StackIsEmpty(stack_ty* stack)
{
    return (0 == StackSize(stack));
}

size_t StackCapacity(stack_ty* stack)
{
    return ((stack -> end - stack -> start) / stack -> element_size);
}

