#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sort.h"
#include "l_sort.h"

typedef void (*sort_func_ty)(int* arr, size_t size, cmp_func_ty func);
typedef void (*lsort_func_ty)(int* arr, size_t size);

static void TestSort(sort_func_ty sort, const char* sort_name);
static void TestLSort(lsort_func_ty sort, const char* sort_name);

int comp(const void* a, const void* b);

int main()
{
	TestSort(BubbleSort, "BubbleSort");
	TestSort(SelectionSort, "SelectionSort");
	TestSort(InsertionSort, "InsertionSort");
	TestLSort(CountSort, "CountSort");
	TestLSort(RadixSort, "RadixSort");
	
	return 0;
}

static void TestSort(sort_func_ty sort, const char* sort_name)
{
	int arr[100] = {0};
	int expected[100] = {0};
	size_t size = sizeof(arr) / sizeof(int);
	size_t i = 0;
	int min = 0, max = 100;
	clock_t cpu_rounds = 0;
	for (i = 0; i < size; ++i)
	{
		arr[i] = rand() % (max - min + 1) + min;
		expected[i] = arr[i];
	}
	
	cpu_rounds = clock();
	qsort(expected, size, sizeof(arr[0]), comp);
	printf("qsort time of run: %ld\n",clock() - cpu_rounds);
	
	cpu_rounds = clock();
	sort(arr, size, comp);
	printf("%s time of run: %ld\n\n", sort_name, clock() - cpu_rounds);
	
	for (i = 0; i < size; ++i)
	{
		if (arr[i] != expected[i])
		{
			printf("\n");
			printf("nigga, function %s idx %ld, arr: %d, expected: %d\n\n", sort_name, i, arr[i], expected[i]);
			for (i = 0; i < size; ++i)
			{
				printf("actual: %d, expected: %d\n", arr[i], expected[i]);
			}
			break;
		}
	}
}

static void TestLSort(lsort_func_ty sort, const char* sort_name)
{
	int arr[100] = {0};
	int expected[100] = {0};
	size_t size = sizeof(arr) / sizeof(int);
	size_t i = 0;
	int min = 0, max = 100;
	clock_t cpu_rounds = 0;
	for (i = 0; i < size; ++i)
	{
		arr[i] = rand() % (max - min + 1) + min;
		expected[i] = arr[i];
	}
	
	cpu_rounds = clock();
	qsort(expected, size, sizeof(arr[0]), comp);
	printf("qsort time of run: %ld\n",clock() - cpu_rounds);
	
	cpu_rounds = clock();
	sort(arr, size);
	printf("%s time of run: %ld\n\n", sort_name, clock() - cpu_rounds);
	
	for (i = 0; i < size; ++i)
	{
		if (arr[i] != expected[i])
		{
			printf("\n");
			printf("nigga, function %s idx %ld, arr: %d, expected: %d\n\n", sort_name, i, arr[i], expected[i]);
			for (i = 0; i < size; ++i)
			{
				printf("actual: %d, expected: %d\n", arr[i], expected[i]);
			}
			break;
		}
	}
}

int comp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}


