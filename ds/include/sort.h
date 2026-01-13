#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h> /* size_t */

typedef int (*cmp_func_ty)(const void* a, const void* b);

void BubbleSort(int* arr, size_t size, cmp_func_ty comp_func);

void SelectionSort(int* arr, size_t size, cmp_func_ty comp_func);

void InsertionSort(int* arr, size_t size, cmp_func_ty comp_func);

#endif  /* __SORT_H__ */
