#ifndef __UTILS_H__
#define __UTILS_H__

#define BAD_MEM(type) 			((type)0xDEADBEEFDEADBEEF)

#if !DNDEBUG
	#define DEBUG_ONLY(expression) do {expression;} while(0);
	#define DEBUG_BAD_MEM(ptr, type) DEBUG_ONLY(ptr = BAD_MEM(type))
#else
	#define DEBUG_ONLY(expression)
	#define DEBUG_BAD_MEM(ptr, type)
#endif


#define SUCCESS		0
#define FAIL		1
#define TRUE		1
#define FALSE		0

#define ARRAY_LEN(arr)	(sizeof(arr) / sizeof(arr[0]))

#endif /*__UTILS_H__*/


