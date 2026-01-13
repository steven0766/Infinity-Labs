#ifndef __L_SORT_H__
#define __L_SORT_H__

#include <stddef.h> /* size_t */

typedef int (*func_cmp_ty)(const void* a, const void* b);

void CountSort(int* arr, size_t size, func_cmp_ty func);

void RadixSort(int* arr, size_t size, func_cmp_ty func);


#endif  /* __L_SORT_H__ */
