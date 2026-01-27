/***************************************
* Reviewer: Steve
* Date: 26.01.26
* Developer: Shalev
* Version: 1
****************************************/

#include <string.h>	/* strncmp */
#include <stdio.h>

#include "recursion.h"

static int FibHelper(int curr, int prev, int idx);
/*static node_ty* FlipHelper(node_ty* curr, node_ty* prev);*/
static void SortedInsert(stack_ty* stk, void* data);

int Fibonacci(int element_index)
{
	if (element_index == 0)
	{
		return 0;
	}
	
	return FibHelper(1, 0, element_index - 1);
}

static int FibHelper(int curr, int prev, int idx)
{
	if (0 == idx)
	{
		return curr;
	}
	
	return FibHelper(curr + prev, curr, idx - 1);
}

int FibonacciIter(int element_index)
{
	int prev1 = 1, prev2 = 0, curr = 0, i = 0;
	
	for (i = 0; i < element_index; ++i)
	{
		printf("FibonacciIter i = %d\n", i + 1);
		prev2 = prev1; 
		prev1 = curr;
		curr = prev1 + prev2;
	}
	
	return curr;
}
#if 0
another version: 

node_ty* FlipList(node_ty* node)
{
	return FlipHelper(node, NULL);
}

static node_ty* FlipHelper(node_ty* curr, node_ty* prev)
{
	node_ty* next = NULL;
	
	if (!curr)
	{
		return prev;
	}
	
	next = curr->next;
	curr->next = prev;
	prev = curr;
	curr = next;
	
	return FlipHelper(curr, prev);
}
#endif

node_ty* FlipList(node_ty* node)
{
	node_ty* new_head = NULL;
	
	if (!node || !node->next)
	{
		return node;
	}
	
	new_head = FlipList(node->next);
	
	node->next->next = node;
	node->next = NULL;
	
	return new_head;
}


void StackSort(stack_ty* stk)
{
	void* val = NULL;
	
	if (StackIsEmpty(stk))
	{
		return;
	}
	
	val = StackPeek(stk);
	
	StackPop(stk);
	
	StackSort(stk);
	
	SortedInsert(stk, val);
	
}

static void SortedInsert(stack_ty* stk, void* data)
{
	int val = 0;
	
	if (StackIsEmpty(stk) || *(int*)data < *(int*)StackPeek(stk))
	{
		StackPush(stk, data);
		return;
	}
	
	val = *(int*)StackPeek(stk);
	
	StackPop(stk);
	
	SortedInsert(stk, data);
	
	StackPush(stk, &val);
	
}

size_t StrLen(const char* str)
{
	return *str ? 1 + StrLen(++str) : 0;
}

int StrCmp(const char* str1, const char* str2)
{
	
	if (!*str1 || *str1 != *str2)
	{
		return *str1 - *str2;
	}
	
	return StrCmp(++str1, ++str2);

}

char* StrCpy(char* dest, const char* src)
{
	if (!*src)
	{
		*dest = '\0';
	}
	else
	{
		*dest = *src;
		StrCpy(dest + 1, src + 1);
	}
	
	return dest;
}

char* StrCat(char* dest, const char* src)
{
	if (!*dest)
	{
		StrCpy(dest, src);
		return dest;
	}
	
	StrCat(dest + 1, src);
	
	return dest;
	
}

char* StrStr(const char* src, const char* target)
{
	size_t dest_len = strlen(target);
	size_t src_len = strlen(src);
	
	if (src_len < dest_len)
	{
		return NULL;
	}
	
	if (0 == strncmp(src, target, dest_len))
	{
		return (char*)src;
	}	
	
	return StrStr(src + 1, target);
}



