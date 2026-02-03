#include <stdio.h>			/* printf */

#include "avl.h"			/* traverse_ty */
#include "test_utils.h"		/* TestSuiteBegin, ASSERT_NOT_NULL, ASSERT_INT */
#define SUCCESS			0
#define FAIL			1
#define ARRAY_LEN(arr)		(sizeof((arr)) / sizeof((arr[0])))
#define LARGE_AVL_INPUTS {50, 25, 10, 5, 11, 30, 27, 28,\
 						  35, 75, 60, 55, 70, 90, 80, 100} 

typedef struct foreach
{
	int arr[5];
	int* idx;
}f_e_check;

static void TestAvlCreate(void);
static void TestInsertAndRemove(void);
static void TestAvlBalance(void);
static void TestForEach(void);

static int CmpFunc(const void* data1, const void* data2, void* param);
static int WriteAction(void* data, void* param);

int main()
{
	TestSuiteBegin("bst");
	printf("========================================\n");
	printf("test for create:\n");
	TestAvlCreate();
	
	printf("\ntest for insert and remove\n");
	printf("________________________________________\n");
	TestInsertAndRemove();
	
	printf("\ntest for balance\n");
	printf("________________________________________\n");
	TestAvlBalance();
	
	printf("\ntest foreach\n");
	printf("________________________________________\n");
	TestForEach();
	
	return 0;
}

static void TestAvlCreate(void)
{
	avl_ty* avl = AvlCreate(CmpFunc, NULL);
	
	ASSERT_NOT_NULL(avl);
	ASSERT_INT(1, AvlIsEmpty(avl));
	
	AvlDestroy(avl);
}

static void TestInsertAndRemove(void)
{
	avl_ty* avl = AvlCreate(CmpFunc, NULL);
	size_t i = 0;
	int result = 0;
	int inputs[] = LARGE_AVL_INPUTS;
	int false_input = 111;
	void* is_found = NULL;
	printf("---create avl---\n");
	ASSERT_NOT_NULL(avl);
	
	printf("---insert data---");
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		result = AvlInsert(avl, &inputs[i]);
		ASSERT_INT(SUCCESS, result);
	}

	printf("---find all data---");
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		is_found = AvlFind(avl, &inputs[i]);
		ASSERT_INT(inputs[i], *(int*)is_found);
	}
	
	printf("---avl size---");
	ASSERT_INT(16, AvlSize(avl));
	
	printf("---avl remove---");
	ASSERT_INT(1, AvlRemove(avl, &false_input));
	
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		is_found = AvlFind(avl, &inputs[i]);
		ASSERT_INT(0, AvlRemove(avl, &inputs[i]));
		ASSERT_INT(16-i-1, AvlSize(avl));
	}
	
	AvlDestroy(avl);
}

static void TestAvlBalance(void)
{
	avl_ty* avl = AvlCreate(CmpFunc, NULL);
	int inputs[] = {1,2,3,4,5};
	size_t i = 0;
	
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		AvlInsert(avl, &inputs[i]);
	}
	
	ASSERT_INT(2, AvlHeight(avl));
	
	AvlRemove(avl, &inputs[0]);
	ASSERT_INT(2, AvlHeight(avl));
	
	AvlRemove(avl, &inputs[1]);
	ASSERT_INT(1, AvlHeight(avl));
	
	AvlDestroy(avl);
}

static void TestForEach(void)
{
	avl_ty* avl = AvlCreate(CmpFunc, NULL);
	int inputs[5] = {0};
	f_e_check p = {0};
	
	AvlForEach(avl, PRE_ORDER, WriteAction, &p);
	
	ASSERT_INT_ARRAY(inputs, p.arr, 5);
	
}

static int CmpFunc(const void* data1, const void* data2, void* param)
{
	(void)param;
	return *(int*)data1 - *(int*)data2;
}

static int WriteAction(void* data, void* param)
{
	f_e_check* p = (f_e_check*)param;
	p->arr[*(p->idx)] = *(int*)data;
	*(p->idx) += 1;
	
	return 0;
}
