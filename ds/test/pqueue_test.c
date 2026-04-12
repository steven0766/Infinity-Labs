#include <stdio.h>		/* printf */
#include "pqueue.h"		/* queue api */

#include "test_utils.h"	

#define UNUSED(var)	((void)var)
#define ARRAY_LEN(arr)	(sizeof((arr)) / sizeof((arr[0])))

static void TestPQCreate(void);
static void TestPQEnqueue(void);
static void TestPQDequeue(void);
static void TestPQRemove(void);

static int IsMatch(const void* data, void* param);
static int CompareAscending(const void* data1, const void* data2, void* param);

int main()
{
	TestSuiteBegin("Priority Queue");
	
	TestPrintSection("Creation Tests");
	TestPQCreate();
	
	TestPrintSection("Enqueue Tests");
	TestPQEnqueue();
	
	TestPrintSection("Dequeue Tests");
	TestPQDequeue();

	TestPrintSection("Remove Tests");
	TestPQRemove();

	return TestSuiteEnd();
}

static void TestPQCreate(void)
{
	pq_ty* pq = PQCreate(CompareAscending, NULL);
	if (!ASSERT_NOT_NULL(pq))
	{
		return;
	}
	
	ASSERT_SIZE(0, PQCount(pq));
    	ASSERT_TRUE(PQIsEmpty(pq));

	PQDestroy(pq);
}

static void TestPQEnqueue(void)
{
	static int inputs[] = {5, 3, 2, 4, 1};
	size_t i = 0;
	
	pq_ty* pq = PQCreate(CompareAscending, NULL);
	if (!ASSERT_NOT_NULL(pq))
	{
		return;
	}
	
	for(i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		ASSERT_SIZE(i, PQCount(pq));
		ASSERT_SUCCESS(PQEnqueue(pq, &inputs[i]));
		ASSERT_SIZE(i + 1, PQCount(pq));
	}
	
	PQDestroy(pq);
}

static void TestPQDequeue(void)
{
	static int inputs[] = {5, 3, 2, 4, 1};
	static int expected[] = {1, 2, 3, 4, 5};
	size_t i = 0;
	
	pq_ty* pq = PQCreate(CompareAscending, NULL);
	if (!ASSERT_NOT_NULL(pq))
	{
		return;
	}
	
	for(i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		ASSERT_SUCCESS(PQEnqueue(pq, &inputs[i]));
	}
	ASSERT_SIZE(ARRAY_LEN(inputs), PQCount(pq));
	
	for(i = 0; i < ARRAY_LEN(expected); ++i)
	{
		ASSERT_INT(expected[i], *(int*)PQDequeue(pq));
		ASSERT_SIZE(ARRAY_LEN(expected) - (i + 1), PQCount(pq));
	}
	ASSERT_TRUE(PQIsEmpty(pq));
	
	PQDestroy(pq);
}

static void TestPQRemove(void)
{
	static int inputs[] = {5, 3, 2, 4, 1};
	size_t i = 0;
	int t1 = 1, t2 = 3, t3 = 5, t4 = 12;
	
	pq_ty* pq = PQCreate(CompareAscending, NULL);
	if (!ASSERT_NOT_NULL(pq))
	{
		return;
	}
	
	for(i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		ASSERT_SUCCESS(PQEnqueue(pq, &inputs[i]));
	}
	
	ASSERT_INT(1, *(int*)PQPeek(pq));
    
    	ASSERT_INT(1, *(int*)PQRemove(pq, IsMatch, &t1));
    	ASSERT_SIZE(4, PQCount(pq));
    	ASSERT_INT(2, *(int*)PQPeek(pq));
    
    	ASSERT_INT(3, *(int*)PQRemove(pq, IsMatch, &t2));
    	ASSERT_INT(2, *(int*)PQPeek(pq));
    	ASSERT_SIZE(3, PQCount(pq));
    
    	ASSERT_INT(5, *(int*)PQRemove(pq, IsMatch, &t3));
    	ASSERT_INT(2, *(int*)PQPeek(pq));
    	ASSERT_SIZE(2, PQCount(pq));
    	
    	ASSERT_NULL(PQRemove(pq, IsMatch, &t4));
	
	PQDestroy(pq);
}

static int CompareAscending(const void* data1, const void* data2, void* param)
{
	UNUSED(param);
	return *(int*)data1 - *(int*)data2;
}

static int IsMatch(const void* data, void* param)
{
	int* num = (int*)data;
	int* target = (int*)param;
	
	return *num == *target;
}

