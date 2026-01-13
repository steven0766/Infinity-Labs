#include "sort.h"

#include <stdio.h>

static void Swap(int* a, int* b);

void BubbleSort(int* arr, size_t size, cmp_func_ty comp_func)
{
	size_t i = 0, j = 0;
	int swapped = 0;
	
	if (size < 2)
	{
	    return;
	}
	
	for (i = 0; i < size - 1; ++i)
	{
		swapped = 0;
		for (j = 0; j < size - i - 1; ++j)
		{
			if (0 < comp_func(&arr[j], &arr[j+1]))
			{
				Swap(&arr[j], &arr[j+1]);
				swapped = 1;
			}
		}
		
		if (0 == swapped)
		{
			break;
		}
	}
}

void SelectionSort(int* arr, size_t size, cmp_func_ty comp_func)
{
	size_t i = 0, j = 0;
	size_t min_idx = 0;
	
	if (size < 2)
	{
	    return;
	}
	
	for (i = 0; i < size - 1; ++i)
	{
		min_idx = i;
		for (j = i; j < size; ++j)
		{
			if (0 > comp_func(&arr[j], &arr[min_idx]))
			{
				min_idx = j;
			}
		}
		Swap(&arr[i], &arr[min_idx]);
	}
}

void InsertionSort(int* arr, size_t size, cmp_func_ty comp_func)
{
	size_t i = 0;
	int j = 0;
	int key = 0;
	
	if (size < 2)
	{
	    return;
	}
	
	for (i = 1; i < size; ++i)
	{
		key = arr[i];
		j = i - 1;
		while (j >= 0 && 0 < comp_func(&arr[j], &key))
		{
			arr[j+1] = arr[j];
			--j;
		}
		
		arr[j + 1] = key;
	}
}

static void Swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
