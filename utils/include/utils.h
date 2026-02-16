#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef NDEBUG
	#define DEBUG_ONLY(expression) do {expression} while(0);
#else
	#define DEBUG_ONLY(expression)
#endif

#define BAD_MEM(type) ((type)NULL)

#endif /*__UTILS_H__*/
