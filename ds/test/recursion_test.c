#include <string.h>		/* strlen, strcmp, strcpy, strcat, strstr */
#include <stdlib.h>		/* malloc, free */
#include "recursion.h"
#include "test_utils.h"
#include "stack.h"

#define ARRAY_LEN(arr)		(sizeof((arr)) / sizeof((arr[0])))


typedef int (*fib_func_ty) (int);

static void TestFibonacci(void);
static void TestFibFunc(fib_func_ty func);
static void TestFlipSlist(void);
static void TestStackSort(void);
static void TestStrLen(void);
static void TestStrCmp(void);
static void TestStrCpy(void);
static void TestStrCat(void);
static void TestStrStr(void);

static node_ty* CreateNode(void* data);

int main()
{
	TestSuiteBegin("Recursion");
	
	TestPrintSection("Fibonacci");
	TestFibonacci();
	
	TestPrintSection("Flip List");
	TestFlipSlist();
	
	TestPrintSection("Sort Stack");
	TestStackSort();
	
	TestPrintSection("StrLen");
	TestStrLen();
	
	TestPrintSection("StrCmp");
	TestStrCmp();
	
	TestPrintSection("StrCpy");
	TestStrCpy();
	
	TestPrintSection("StrCat");
	TestStrCat();
	
	TestPrintSection("StrStr");
	TestStrStr();
	
	return TestSuiteEnd();
}




static void TestFibonacci(void)
{
	TestFibFunc(Fibonacci);
	TestFibFunc(FibonacciIter);
}

static void TestFibFunc(fib_func_ty func)
{
	size_t i = 0;
	int expected[] = {0, 1, 1, 2, 3, 5, 8, 13};
	
	for (i = 0; i < ARRAY_LEN(expected); ++i)
	{
		ASSERT_INT(expected[i], func(i));
	}
}

static void TestFlipSlist(void)
{
	node_ty* head = NULL, *new_head = NULL;
	node_ty* curr = NULL, *prev = NULL;
	int actual[] = {1, 2, 3, 4, 5, 6};
	int expected[] = {6, 5, 4, 3, 2, 1};
	size_t i = 0;
	
	head = CreateNode(&actual[0]);
	prev = head;
	curr = head->next;
	
	for (i = 1; i < ARRAY_LEN(actual); ++i)
	{
		curr = CreateNode(&actual[i]);
		prev->next = curr;
		prev = curr;
		curr = curr->next;
	}
	curr = head;
	curr = FlipList(curr);
	new_head = curr;
	
	for (i = 0; i < ARRAY_LEN(expected); ++i)
	{
		ASSERT_INT(expected[i], *(int*)(curr->data));
		curr = curr->next;
	}
	curr = new_head;
	while (curr)
	{
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	
}

static void TestStackSort(void)
{
	int inputs[10] = {5, 8, 3, 2, 10, 14, 12, 1, 4, 11};
	int expected[10] = {1, 2, 3, 4, 5, 8, 10, 11, 12, 14};
	size_t i = 0;
	
	stack_ty* stack = StackCreate(ARRAY_LEN(inputs), sizeof(int));
	
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		StackPush(stack, &inputs[i]);
	}
	
	StackSort(stack);
	
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		ASSERT_INT(expected[i], *(int*)StackPeek(stack));
		StackPop(stack);
	}
	
}

static void TestStrLen(void)
{
	const char* empty = "";
	const char* str = "123456";
	ASSERT_SIZE(strlen(empty), StrLen(empty));
	ASSERT_SIZE(strlen(str), StrLen(str));
}

static void TestStrCmp(void)
{
	const char* str1 = "hello_world";
	const char* str2 = "different";
	const char* str3 = "hello_world";
	
	ASSERT_INT(strcmp(str1, str2), StrCmp(str1, str2));
	ASSERT_INT(strcmp(str1, str3), StrCmp(str1, str3));
}

static node_ty* CreateNode(void* data)
{
	node_ty* node = (node_ty*)malloc(sizeof(*node));
	if (!node)
	{
		return NULL;
	}
	
	node->data = data;
	node->next = NULL;
	
	return node;
	
}

static void TestStrCpy(void)
{
	char dest_actual[10];
	char dest_expected[10];
	char* expected = NULL;
	char* actual = NULL;
	
	const char* src = "123456789";	
	
	actual = StrCpy(dest_actual, src);
	expected = strcpy(dest_expected, src);
	ASSERT_STR_EQUAL(expected, actual);
	
}

static void TestStrCat(void)
{
	const char* src = " World";
	char expected[20] = {0};
	char actual[20] = {0};
	strcpy(expected, "Hello");
	strcpy(actual, "Hello");
	
	ASSERT_STR_EQUAL(strcat(expected, src), StrCat(actual, src));
}

static void TestStrStr(void)
{
	const char* src = "Hello World";
	const char* str1 = "ello";
	const char* str2 = "ellg";
	
	ASSERT_STR_EQUAL(strstr(src, str1), StrStr(src, str1));
	ASSERT_STR_EQUAL(strstr(src, str2), StrStr(src, str2));
}
