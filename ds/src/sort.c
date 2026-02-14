#include <stdlib.h>/* size_t, calloc, malloc, free */
#include <string.h>/* memcpy */
#include <assert.h>/* assert */
#include <sys/types.h>/* ssize_t */

#include "sort.h"

#define SUCCESS		0
#define FAIL		1

static void Swap(int* a, int* b);
static int* CountNumbers(int* arr, size_t size);
static int FindMaxInArray(int* arr, size_t len);
static void PrefixSum(int* count_arr, size_t size);
static void RadixCountByDigit(int arr[], int n, int exp);
static int RadixGetMax(int arr[], size_t n);

static ssize_t BinarySearchRecursive(int* arr, ssize_t left, ssize_t right, int target);

static void MergeSortSplit(int* arr, size_t left, size_t right, int* tmp);
static void Merge(int* arr, size_t left, size_t mid, size_t right, int* tmp);

static size_t Partition(void* arr, ssize_t left, ssize_t right, size_t el_size, cmp_func_ty cmp);
static void QuickSortHelper(void* arr, ssize_t left, ssize_t right, size_t el_size, cmp_func_ty cmp);

static node_ty* MergeSortedLists(node_ty* n1, node_ty* n2);
static node_ty* GetMiddle(node_ty* head);

static void SwapByByte(char* a, char* b, size_t el_size);

static int Max(int a, int b);

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

void CountSort(int* arr, size_t size)
{
	size_t max_num = 0;
	int* count_arr = NULL;
	int* ans = NULL;
	int i = 0;
	
	max_num = FindMaxInArray(arr, size);
	count_arr = CountNumbers(arr, size);
	
	PrefixSum(count_arr, max_num + 1);
	
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

void RadixSort(int* arr, size_t size)
{
	int max = RadixGetMax(arr, size);
	int exp = 0;
	
	if (size < 2)
	{
	    return;
	}
	
	for (exp = 1; max / exp > 0; exp *= 10)
	{
		RadixCountByDigit(arr, size, exp);
	}
}

ssize_t BinarySearchIter(int* arr, size_t len, int target)
{
	ssize_t left = 0;
	ssize_t right = len - 1;
	ssize_t mid = 0;
	
	while (left <= right)
	{
		mid = left + (right - left) / 2;
		
		if (arr[mid] == target)
		{
			return mid;
		}
		else if(arr[mid] > target)
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}
	
	return -1;
}

ssize_t BinarySearch(int* arr, size_t len, int target)
{
	if (!len)
	{
		return -1;
	}
	
	return BinarySearchRecursive(arr, 0, len - 1, target);
}



int MergeSort(int* arr, size_t len)
{
	int* tmp = NULL;
	
	assert(NULL != arr);

	tmp = (int*)malloc(sizeof(int) * len);
	if (NULL == tmp)
	{
		return FAIL;
	}
	
	MergeSortSplit(arr, 0, len - 1, tmp);
	
	free(tmp);
	tmp = NULL;
	
	return SUCCESS;
}

void QuickSort(void* arr, size_t len, size_t el_size, cmp_func_ty cmp)
{	
	assert(NULL != arr);
	assert(0 != el_size);
	
	QuickSortHelper(arr, 0, len - 1, el_size, cmp);
}

node_ty* SLLMergeSort(node_ty* head)
{
	node_ty* mid = NULL;
	node_ty* left = NULL;
	node_ty* right = NULL;
	node_ty* right_half = NULL;
	
	if (!head || !head->next)
	{
		return head;
	}
	
	mid = GetMiddle(head);
	right_half = mid->next;

	mid->next = NULL;
	
	left = SLLMergeSort(head);

	right = SLLMergeSort(right_half);

	return MergeSortedLists(left, right);
}

static void QuickSortHelper(void* arr, ssize_t left, ssize_t right, size_t el_size, cmp_func_ty cmp)
{
	ssize_t pivot = 0;
	
	if (left >= right)
	{
		return;
	}
	
	pivot = Partition(arr, left, right, el_size, cmp);
	
	QuickSortHelper(arr, left, pivot - 1, el_size, cmp);
	QuickSortHelper(arr, pivot + 1, right, el_size, cmp);
	
}

static size_t Partition(void* arr, ssize_t left, ssize_t right, size_t el_size, cmp_func_ty cmp)
{
	ssize_t pivot = 0;
	ssize_t i = left - 1;
	
	pivot = right;
	
	for (; left < pivot; ++left)
	{
		if (cmp((char*)arr + (el_size * left), (char*)arr + (el_size * pivot)) < 0)
		{
			++i;
			SwapByByte((char*)arr + (el_size * i), (char*)arr + (el_size * left), el_size);
		}	
	}
	
	++i;
	SwapByByte((char*)arr + (el_size * i), (char*)arr + (el_size * pivot), el_size);
		
	return i;
}

static int FindMaxInArray(int* arr, size_t len)
{
	int max_el = arr[0];
	size_t i = 0;
	
	for (i = 0; i < len; ++i)
	{
		max_el = Max(max_el, arr[i]);
	}
	
	return max_el;
}

static node_ty* GetMiddle(node_ty* head)
{
	node_ty* slow = head;
	node_ty* fast = NULL;
	
	if (!head)
	{
		return NULL;
	}
	
	fast = head->next;
	
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	
	return slow;
}

static node_ty* MergeSortedLists(node_ty* n1, node_ty* n2)
{
	node_ty dummy = {0};
	node_ty* curr = &dummy;

	while (n1 && n2)
	{
		if (n1->data < n2->data)
		{
			curr->next = n1;
			n1 = n1->next;
		}
		else
		{
			curr->next = n2;
			n2 = n2->next;
		}
		curr = curr->next; 
	}
	
	curr->next = n1 ? n1 : n2;

	return dummy.next;
}


static void Swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
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
	
	for (i = 0; i < size; ++i)
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

static int RadixGetMax(int arr[], size_t n)
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

static ssize_t BinarySearchRecursive(int* arr, ssize_t left, ssize_t right, int target)
{
	ssize_t mid = left + (right - left) / 2;
	
	assert(NULL != arr);
	
	if (left > right)
	{
		return -1;
	}
	
	if (arr[mid] == target)
	{
		return mid;
	}
	else if (arr[mid] < target)
	{
		return BinarySearchRecursive(arr, mid + 1, right, target);
	}
	else
	{
		return BinarySearchRecursive(arr, left, mid - 1, target);
	}
	
}

static void MergeSortSplit(int* arr, size_t left, size_t right, int* tmp)
{
	size_t mid = left + (right - left) / 2;

	if (left >= right)
	{
		return;
	}
	
	MergeSortSplit(arr, left, mid, tmp);
	MergeSortSplit(arr, mid + 1, right, tmp);

	Merge(arr, left, mid, right, tmp);
}

static void Merge(int* arr, size_t left, size_t mid, size_t right, int* tmp)
{
	size_t left_idx = left;
	size_t right_idx = mid + 1;
	size_t tmp_idx = left;
	size_t i = 0;
	
	while (left_idx <= mid && right_idx <= right)
	{
		if (arr[left_idx] < arr[right_idx])
		{
			tmp[tmp_idx] = arr[left_idx++];
		}
		else
		{
			tmp[tmp_idx] = arr[right_idx++];
		}
		++tmp_idx;
	}
	
	/* copy leftovers */
	while (left_idx <= mid)
	{
		tmp[tmp_idx++] = arr[left_idx++];
	}
	
	while (right_idx <= right)
	{
		tmp[tmp_idx++] = arr[right_idx++];
	}
	
	/* copy back to original */
	
	for (i = left; i <= right; ++i)
	{
		arr[i] = tmp[i];
	}
}

static void SwapByByte(char* a, char* b, size_t el_size)
{
	size_t i = 0;
	char tmp = 0;

	for (i = 0; i < el_size; ++i, ++a, ++b)
	{
		tmp = *a;
		*a = *b;
		*b = tmp; 
		
	}
}

static int Max(int a, int b)
{
	return a > b ? a : b;
}

