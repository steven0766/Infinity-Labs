#include <stdlib.h>		/* rand, seed, qsort, malloc, free */
#include <time.h>		/* time, clock_t, CLOCKS_PER_SEC, clock */
#include <stdio.h>		/* printf */
#include <string.h>  		/* memcpy */

#include "sort.h"
#include "test_utils.h"

#define TEST_LEN		5000
#define BASE_MIN		-10000
#define	BASE_MAX		10000
#define ARRAY_LEN(arr)		(sizeof((arr)) / sizeof((arr[0])))

typedef void (*cmp_sort_func)(int* arr, size_t len, cmp_func_ty f);
typedef void (*linear_sort_func)(int* arr, size_t len);
typedef ssize_t (*binary_search_func)(int* arr, size_t len, int target);

static void TestComparisonSort(cmp_sort_func sf, const char* name);
static void TestLinearSort(linear_sort_func sf, const char* name);
static void TestBinarySearch(binary_search_func bs, const char* name);
static void TestMergeSort(void);
static void TestQuickSort(void);
static void TestSLLMergeSort(void);

static int VerifySorted(int* arr, size_t len);
static int VerifySLLSorted(node_ty* head);
static void InitRandomArray(int* dest, size_t len, int min, int max);
static void CopyArray(int* src, int* dest, size_t len);
static void PrintArray(int* arr, size_t len);
static void PrintSLL(node_ty* head);
static node_ty* CreateSLLFromArray(int* arr, size_t len);
static void SLLDestory(node_ty* head);
static node_ty* CreateNode(int data);
	
static void ComparePerformanceCmp(cmp_sort_func sf, const char* name);
static void ComparePerformanceLinear(linear_sort_func sf, const char* name);
static void ComparePerformanceQuickSort(void);

static int Ascending(const void* a, const void* b);

int main()
{
	srand(time(NULL));
	
	TestSuiteBegin("Sorting");
	
	TestPrintSection("Comparison Sorts");
	TestComparisonSort(BubbleSort, "BubbleSort");
	TestComparisonSort(SelectionSort, "SelectionSort");
	TestComparisonSort(InsertionSort, "InsertionSort");
	
	
	TestPrintSection("Linear Sorts");
	TestLinearSort(CountSort, "CountSort");
	TestLinearSort(RadixSort, "RadixSort");
	
	TestPrintSection("Binary Search");
	TestBinarySearch(BinarySearchIter, "Binary Search Iterative");
	TestBinarySearch(BinarySearch, "Binary Search Recursive");
	
	TestPrintSection("Merge Sort");
	TestMergeSort();
	
	TestPrintSection("Merge Sort SLL");
	TestSLLMergeSort();
	
	TestPrintSection("Quick Sort");
	TestQuickSort();

	return TestSuiteEnd();
}

static void TestComparisonSort(cmp_sort_func sf, const char* name)
{
	int arr[TEST_LEN];
	
	printf("Testing %s\n", name);
	
	InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	sf(arr, TEST_LEN, Ascending);
	
	ASSERT_TRUE(VerifySorted(arr, TEST_LEN));

	ComparePerformanceCmp(sf, name);
	
	printf("\n");
}

static void TestLinearSort(linear_sort_func sf, const char* name)
{
	int arr[TEST_LEN];
	
	printf("Testing %s\n", name);
	
	/* Different test ranges for different sorts */
	if (strcmp(name, "CountSort") == 0)
	{
		/* Small range for basic counting sort */
		InitRandomArray(arr, TEST_LEN, 1, 100);
	}
	else if (strcmp(name, "RadixSort") == 0)
	{
		/* 7-digit numbers for radix sort */
		InitRandomArray(arr, TEST_LEN, 1000000, 9999999);
	}
	else
	{
		InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	}
	sf(arr, TEST_LEN);
	
	ASSERT_TRUE(VerifySorted(arr, TEST_LEN));
	
	/* Performance */
	ComparePerformanceLinear(sf, name);
}

static void TestBinarySearch(binary_search_func bs, const char* name)
{
	int arr[] = {1, 3, 5, 7, 8, 9, 10, 11, 12, 15};
	int targets[] = {1, 3, 6, 12, 15, -3, 17};
	ssize_t expected[] = {0, 1, -1, 8, 9, -1, -1};
	size_t i = 0, len = ARRAY_LEN(arr);
	printf("Testing %s\n", name);
	
	for (i = 0; i < ARRAY_LEN(targets); ++i)
	{
		ASSERT_INT(expected[i], bs(arr, len, targets[i]));
	}
}

static void TestMergeSort(void)
{
	int status = 0;
	int arr[TEST_LEN];
	InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	
	status = MergeSort(arr, TEST_LEN);
	
	ASSERT_SUCCESS(status);
	
	ASSERT_TRUE(VerifySorted(arr, TEST_LEN));
}

static void TestQuickSort(void)
{
	int arr[TEST_LEN];
	InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	
	QuickSort(arr, TEST_LEN, sizeof(int), Ascending);
	
	ASSERT_TRUE(VerifySorted(arr, TEST_LEN));
	
	ComparePerformanceQuickSort();
}

static void TestSLLMergeSort(void)
{
	node_ty* head = NULL;
	int arr[TEST_LEN];
	InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	
	head = CreateSLLFromArray(arr, TEST_LEN);
	if (!head)
	{
		return;
	}
	
	head = SLLMergeSort(head);
	
	ASSERT_TRUE(VerifySLLSorted(head));
	
	SLLDestory(head);
}

static int VerifySorted(int* arr, size_t len)
{
	size_t i;
	
	if (len <= 1)
	{
		return 1;
	}
	
	for (i = 0; i < len - 1; ++i)
	{
		if (arr[i] > arr[i + 1])
		{
			printf("    Error at index %lu: %d > %d\n", 
			       i, arr[i], arr[i + 1]);
			return 0;
		}
	}
	
	return 1;
}

static int VerifySLLSorted(node_ty* head)
{
	size_t i = 0;
	
	node_ty* curr = head, *next = head->next;
	
	while (next)
	{
		if (curr->data > next->data)
		{
			printf("    Error at index %lu: %d > %d\n", 
			       i, curr->data, next->data);
			return 0;
		}
		curr = next;
		next = curr->next;
		++i;
	}
	
	return 1;
}

static void InitRandomArray(int* dest, size_t len, int min, int max)
{
	size_t i = 0;
	
	for (; i < len; ++i)
	{
		dest[i] = min + (rand() % (max - min + 1));
	}
}

static void CopyArray(int* src, int* dest, size_t len)
{
	memcpy(dest, src, len * sizeof(int));
}

static void PrintArray(int* arr, size_t len)
{
	size_t i = 0;
	for (; i < len; ++i)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

static void PrintSLL(node_ty* head)
{
	node_ty* curr = head;
	printf("[ ");
	while (curr)
	{
		printf("%d-> ", curr->data);
		curr = curr->next;
	}
	printf("NULL] \n");
}

static node_ty* CreateSLLFromArray(int* arr, size_t len)
{
	size_t i = 0;
	node_ty* head = NULL;
	node_ty* curr = NULL;
	node_ty* prev = NULL;
	
	head = CreateNode(arr[0]);
	if (!head)
	{
		return NULL;
	}
	
	curr = head->next;
	prev = head;
	for (i = 1; i < len; ++i)
	{
		curr = CreateNode(arr[i]);
		if (!curr)
		{
			SLLDestory(head);
			head = NULL;
			return NULL;
		}
		prev->next = curr;
		prev = curr;
		curr = curr->next;
	}
	
	return head;
}

static void SLLDestory(node_ty* head)
{
	node_ty* curr = head;
	node_ty* prev = NULL;
	
	while (curr)
	{
		prev = curr;
		curr = curr->next;
		free(prev);
	}
}

static node_ty* CreateNode(int data)
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

static void ComparePerformanceCmp(cmp_sort_func sf, const char* name)
{
	int arr[TEST_LEN];
	int arr_copy[TEST_LEN];
	clock_t start, end;
	double custom_time = 0;
	double qsort_time = 0;
	double ratio = 0;
	
	InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	CopyArray(arr, arr_copy, TEST_LEN);
	
	/* Custom sort */
	start = clock();
	sf(arr, TEST_LEN, Ascending);
	end = clock();
	custom_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	/* qsort */
	start = clock();
	qsort(arr_copy, TEST_LEN, sizeof(int), Ascending);
	end = clock();
	qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	ratio = (qsort_time > 0) ? custom_time / qsort_time : 0;
	
	printf("  Performance: %s=%.4fs | qsort=%.4fs | ratio=%.2fx\n", 
	       name, custom_time, qsort_time, ratio);
}

static void ComparePerformanceLinear(linear_sort_func sf, const char* name)
{
	int arr[TEST_LEN];
	int arr_copy[TEST_LEN];
	clock_t start, end;
	double custom_time = 0;
	double qsort_time = 0;
	double ratio = 0;
	
	/* Use appropriate range for each algorithm */
	if (strcmp(name, "RadixSort") == 0)
	{
		InitRandomArray(arr, TEST_LEN, 1000000, 9999999);
	}
	else if (strcmp(name, "CountingSortAdvanced") == 0)
	{
		InitRandomArray(arr, TEST_LEN, -50, 50);
	}
	else
	{
		InitRandomArray(arr, TEST_LEN, 1, 100);
	}
	
	CopyArray(arr, arr_copy, TEST_LEN);
	
	/* Custom sort */
	start = clock();
	sf(arr, TEST_LEN);
	end = clock();
	custom_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	/* qsort */
	start = clock();
	qsort(arr_copy, TEST_LEN, sizeof(int), Ascending);
	end = clock();
	qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	ratio = (qsort_time > 0) ? custom_time / qsort_time : 0;
	
	printf("  Performance: %s=%.4fs | qsort=%.4fs | ratio=%.2fx\n", 
	       name, custom_time, qsort_time, ratio);
}

static void ComparePerformanceQuickSort(void)
{
	int arr[TEST_LEN];
	int arr_copy[TEST_LEN];
	clock_t start, end;
	double custom_time = 0;
	double qsort_time = 0;
	double ratio = 0;
	
	InitRandomArray(arr, TEST_LEN, BASE_MIN, BASE_MAX);
	CopyArray(arr, arr_copy, TEST_LEN);
	
	/* Custom quick sort */
	start = clock();
	QuickSort(arr, TEST_LEN, sizeof(int), Ascending);
	end = clock();
	custom_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	/* qsort */
	start = clock();
	qsort(arr_copy, TEST_LEN, sizeof(int), Ascending);
	end = clock();
	qsort_time = (double)(end - start) / CLOCKS_PER_SEC;
	
	ratio = (qsort_time > 0) ? custom_time / qsort_time : 0;
	
	printf("  Performance: MyQuickSort=%.4fs | qsort=%.4fs | ratio=%.2fx\n", 
	        custom_time, qsort_time, ratio);
}

static int Ascending(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}


