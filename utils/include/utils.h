#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef NDEBUG
	#define DEBUG_ONLY(expression) do {expression} while(0);
#else
	#define DEBUG_ONLY(expression)
#endif

#define BAD_MEM(type) ((type)NULL)

#define SUCCESS		0
#define FAIL		1
#define TRUE		1
#define FALSE		0

#endif /*__UTILS_H__*/
