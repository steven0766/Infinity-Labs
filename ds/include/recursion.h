#ifndef __ILRD_RECURSION_H__
#define __ILRD_RECURSION_H__
#include "stack.h"

typedef struct node
{
	void* data;
	struct node* next;
}node_ty;

int Fibonacci(int element_index);
int FibonacciIter(int element_index);
node_ty* FlipList(node_ty* node);

void StackSort(stack_ty* stk);

size_t StrLen(const char* str);
int StrCmp(const char* str1, const char* str2);
char* StrCpy(char* dest, const char* src);
char* StrCat(char* dest, const char* src);
char* StrStr(const char* src, const char* target);

#endif	/* __ILRD_RECURSION_H__ */

