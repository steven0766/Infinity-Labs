#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h> /* size_t */
#include <sys/types.h>/* ssize_t */

typedef int (*cmp_func_ty)(const void* a, const void* b);

typedef struct node
{
	int data;
	struct node* next;
}node_ty;

/* ==========================Sort========================== */
void BubbleSort(int* arr, size_t size, cmp_func_ty comp_func);
void SelectionSort(int* arr, size_t size, cmp_func_ty comp_func);
void InsertionSort(int* arr, size_t size, cmp_func_ty comp_func);

/* ======================Linear Sort====================== */
void CountSort(int* arr, size_t size);
void RadixSort(int* arr, size_t size);

/* =================sort and srearch Sort================= */

ssize_t BinarySearchIter(int* arr, size_t len, int target);
ssize_t BinarySearch(int* arr, size_t len, int target);

int MergeSort(int* arr, size_t len);
void QuickSort(void* arr, size_t len, size_t el_size, cmp_func_ty cmp);
node_ty* SLLMergeSort(node_ty* head);


#endif  /* __SORT_H__ */
