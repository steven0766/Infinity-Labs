#include <stdio.h> /* printf */

#include "bst.h"
#include "test_utils.h"

#define ARRAY_LEN(arr)		(sizeof((arr)) / sizeof((arr[0])))
#define LARGE_BST_INPUTS {50, 25, 10, 5, 11, 30, 27, 28,\
 						  35, 75, 60, 55, 70, 90, 80, 100} /* 16 */
#define SORTED_INPUTES {5, 10, 11, 25, 27, 28, 30, 35,\
						50, 55, 60, 70, 75, 80, 90, 100}

static void TestBstCreate(void);
static void TestInsertAndRemove(void);
static void TestNextAndPrev(void);
static void TestForEach(void);

static int CmpFunc(const void* data1, const void* data2, void* param);
static int SumFunc(void* data, void* param);
static int FailFunc(void* data, void* param);

int main()
{
	TestSuiteBegin("bst");
	printf("========================================\n");
	printf("test for create:\n");
	printf("________________________________________\n");
	TestBstCreate();
	
	printf("\ntest for insert and remove\n");
	printf("________________________________________\n");
	TestInsertAndRemove();
	
	printf("\ntest for next and prev\n");
	printf("________________________________________\n");
	TestNextAndPrev();
	
	printf("\ntest for foreach\n");
	printf("________________________________________\n");
	TestForEach();
	
	return 0;
}

static void TestBstCreate(void)
{
	bst_ty* bst = BstCreate(CmpFunc, NULL);

	ASSERT_NOT_NULL(bst);
	ASSERT_INT(1, BstIsEmpty(bst));
	
	BstDestroy(bst);
}

static void TestInsertAndRemove(void)
{
	bst_ty* bst = BstCreate(CmpFunc, NULL);
	int inputs[16] = LARGE_BST_INPUTS;
	size_t i = 0;
	bst_iter_ty iter = NULL;
	bst_iter_ty it_cmp = NULL;
	
	printf("test insert:\n");
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		iter = BstInsert(bst, &inputs[i]);
		ASSERT_NOT_NULL(iter);
		ASSERT_INT(inputs[i], *(int*)BstGetData(iter));
	}
	printf("size:\n");
	ASSERT_SIZE(16, BstSize(bst)); 
	
	/* remove leaf */
	printf("remove leaf:\n");
	iter = BstItBegin(bst);
	ASSERT_INT(5, *(int*)BstGetData(iter));
	iter = BstRemove(iter);
	ASSERT_INT(10, *(int*)BstGetData(iter));
	ASSERT_SIZE(15, BstSize(bst));
	
	/* remove iter with 1 child */
	printf("remove iter with 1 child:\n");
	printf("remove iter with right child\n");
	iter = BstRemove(iter);
	ASSERT_INT(11, *(int*)BstGetData(iter));
	printf("remove iter with left child\n");
	BstInsert(bst, &inputs[3]);
	iter = BstRemove(iter);
	ASSERT_INT(25, *(int*)BstGetData(iter));
	ASSERT_SIZE(14, BstSize(bst));
	
	/* remove iter with 2 childs */
	printf("remove iter with 2 child:\n");
	iter = BstRemove(iter);
	ASSERT_INT(27, *(int*)BstGetData(iter));
	iter = BstFind(bst, &inputs[9]);
	iter = BstRemove(iter);
	ASSERT_INT(80, *(int*)BstGetData(iter));
	ASSERT_SIZE(12, BstSize(bst));
	
	/* remove all iters */
	printf("remove all iters and test equal:\n");
	iter = BstItBegin(bst);
	it_cmp = iter;
	for (i = 0; i < BstSize(bst); ++i)
	{
		ASSERT_INT(1, BstItEqual(it_cmp, iter));
		it_cmp = BstItNext(iter);
		iter = BstRemove(iter);
	}
	
	BstDestroy(bst);
}

static void TestNextAndPrev(void)
{
	bst_ty* bst = BstCreate(CmpFunc, NULL);
	int inputs[16] = LARGE_BST_INPUTS;
	int expected[16] = SORTED_INPUTES;
	size_t i = 0;
	bst_iter_ty iter = NULL;
	
	printf("test insert:\n");
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		iter = BstInsert(bst, &inputs[i]);
		ASSERT_INT(inputs[i], *(int*)BstGetData(iter));
	}
	
	/* test next till the end */
	printf("test next:\n");
	iter = BstItBegin(bst);
	ASSERT_INT(5, *(int*)BstGetData(iter));
	
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		ASSERT_INT(expected[i], *(int*)BstGetData(iter));
		iter = BstItNext(iter);
	}
	
	/* test prev till begin */
	printf("test prev:\n");
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		iter = BstItPrev(iter);
		ASSERT_INT(expected[ARRAY_LEN(inputs) - i - 1], *(int*)BstGetData(iter));
	}
	
	BstDestroy(bst);
}

static void TestForEach(void)
{
	bst_ty* bst = BstCreate(CmpFunc, NULL);
	int inputs[16] = LARGE_BST_INPUTS;
	size_t i = 0;
	bst_iter_ty iter = NULL;
	int actual_sum = 0;
	int sum = 0;
	int test = 3;
	int is_success = 0;
	
	printf("test insert:\n");
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		iter = BstInsert(bst, &inputs[i]);
		ASSERT_INT(inputs[i], *(int*)BstGetData(iter));
	}
	
	/* test foreach pass all time */
	printf("test foreach success\n");
	is_success = BstForEach(BstItBegin(bst), BstItEnd(bst), SumFunc, &sum);
	
	for (i = 0; i < ARRAY_LEN(inputs); ++i)
	{
		actual_sum += inputs[i];
	}
	ASSERT_INT(actual_sum, sum);
	ASSERT_INT(0, is_success);
	
	/* test foreach pass 1 time then fail */
	printf("test foreach fail\n");
	printf("function will fail in:\n");
	is_success = BstForEach(BstItBegin(bst), BstItEnd(bst), FailFunc, &test);
	ASSERT_INT(0, test);
	ASSERT_INT(1, is_success);
	
	
	BstDestroy(bst);
}

static int CmpFunc(const void* data1, const void* data2, void* param)
{
	(void)param;
	return *(int*)data1 - *(int*)data2;
}

static int SumFunc(void* data, void* param)
{
	*(int*)param = *(int*)param + *(int*)data;
	return 0;
}

static int FailFunc(void* data, void* param)
{
	(void)data;
	if (*(int*)param)
	{
		printf("%d\n", *(int*)param);
		*(int*)param = *(int*)param - 1;
		return 0;
	}
	
	printf("BOOM\n");
	return 1;
}
