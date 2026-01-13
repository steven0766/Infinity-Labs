#include <stdlib.h>
#include <string.h>

#include "l_sort.h"

static int* CountNumbers(int* arr, size_t size);
static void PrefixSum(int* count_arr, size_t size);
static void RadixCountByDigit(int arr[], int n, int exp);
static int RadixGetMax(int arr[], int n);

void CountSort(int* arr, size_t size, func_cmp_ty func)
{
	size_t max_num = 101;
	int* count_arr = CountNumbers(arr, size);
	int* ans = NULL;
	int i = 0;
	
	(void)func;
	
	if (size < 2)
	{
	    return;
	}
	
	if (NULL == count_arr)
	{
		return;
	}
	
	PrefixSum(count_arr, max_num);
	ans = (int*)malloc(size * sizeof(int));
	if (NULL == ans)
	{
		return;
	}
	
	/* sort is happening here */
	for (i = size - 1; i >= 0; --i)
	{
		ans[count_arr[arr[i]]-1] = arr[i];
		--count_arr[arr[i]];
	}
	
	memcpy(arr, ans, sizeof(int) * size);
	
	free(count_arr);
	free(ans);
}

static int* CountNumbers(int* arr, size_t size)
{
	size_t max_num = 101;
	size_t i = 0;
	int* count_arr = (int*)calloc(max_num, sizeof(int));
	if (NULL == count_arr)
	{
		return NULL;
	}
	
	for (i = 0; i < (int)size; ++i)
	{
		++count_arr[arr[i]];
	}
	
	return count_arr;
}

static void PrefixSum(int* count_arr, size_t size)
{
	int i = 1;
	
	for (i = 1; i < (int)size; ++ i)
	{
		count_arr[i] += count_arr[i-1];
	}
}

void RadixSort(int* arr, size_t size, func_cmp_ty func)
{
	int max = RadixGetMax(arr, size);
	int exp = 0;
	
	(void)func;
	
	if (size < 2)
	{
	    return;
	}
	
	for (exp = 1; max / exp > 0; exp *= 10)
	{
		RadixCountByDigit(arr, size, exp);
	}
}

static void RadixCountByDigit(int arr[], int n, int exp)
{
	int* count = NULL;
	int* ans = NULL;
	int i = 0;

	count = (int*)calloc(n + 1, sizeof(int));
	if (NULL == count)
	{
		return;
	}
	
	ans = (int*)malloc(sizeof(int) * n);
	if (NULL == ans)
	{
		return;
	}
	
	for (i = 0; i < n; ++i)
	{
		++count[(arr[i] / exp) % 10];
	}
	
	for (i = 1; i < n; ++i)
	{
		count[i] += count[i-1];
	}
	
	for (i = n - 1; i >= 0; --i)
	{
		ans[count[(arr[i] / exp) % 10] - 1] = arr[i];
		--count[(arr[i] / exp) % 10];
	}
	
	for (i = 0; i < n; ++i)
	{
		arr[i] = ans[i];
	}
	
	free(count);
	free(ans);
}

static int RadixGetMax(int arr[], int n)
{
	size_t i = 0;
	int max = arr[0];
	
	for (i = 1; i < n; ++i)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}
	
	return max;
}

