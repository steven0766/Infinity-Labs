#ifndef __STACK_H__
#define __STACK_H__

typedef struct stack_s stack_ty;

#include <stddef.h> /* size_t */

/* creating new stack.
   input: size of array and size of eaach elememnt
   return: stack_ty pointer */
stack_ty* StackCreate(size_t capacity, size_t ele_size);

/* destroy the stack. */
void StackDestroy(stack_ty* stack);

/* take element from stack */
void StackPop(stack_ty* stack);

/* put new element to stack */
void StackPush(stack_ty* stack, void* new_element);

/* loot at the last element we put on stack */
void* StackPeek(stack_ty* stack);

/* what is the size of each element */
size_t StackSize(stack_ty* stack);

/* check if we have any element in stack */
size_t StackIsEmpty(stack_ty* stack);

/* how much elements we have put on stack */
size_t StackCapacity(stack_ty* stack);


#endif   /* __STACK_H__ */
