#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>  /* perror */
#include <stdlib.h> /* exit */

#define BAD_MEM(type) ((type)0xDEADBEEFDEADBEEF)

#define EXIT_IF_BAD(condition, status, msg)                                    \
  do                                                                           \
  {                                                                            \
    if (!(condition))                                                          \
    {                                                                          \
      fprintf(stderr, "%s\n", (msg));                                          \
      exit((status));                                                          \
    }                                                                          \
  } while (0)

#define EXIT_IF_BAD_CLEAN(condition, status, msg, clean_op)                    \
  if (!(condition))                                                            \
  {                                                                            \
    fprintf(stderr, "%s\n", (msg));                                            \
    (clean_op);                                                                \
    exit(status);                                                              \
  }

#define RETURN_IF_BAD(condition, status, msg)                                  \
  do                                                                           \
  {                                                                            \
    if (!(condition))                                                          \
    {                                                                          \
      fprintf(stderr, "%s\n", (msg));                                          \
      return (status);                                                         \
    }                                                                          \
  } while (0)

#define RETURN_IF_BAD_CLEAN(condition, status, msg, clean_op)                  \
  if (!(condition))                                                            \
  {                                                                            \
    fprintf(stderr, "%s\n", (msg));                                            \
    (clean_op);                                                                \
    return (status);                                                           \
  }

#define EXIT_IF_BAD_PERROR(condition, status, msg)                             \
  do                                                                           \
  {                                                                            \
    if (!(condition))                                                          \
    {                                                                          \
      perror((msg));                                                           \
      exit((status));                                                          \
    }                                                                          \
  } while (0)

#define EXIT_IF_BAD_PERROR_CLEAN(condition, status, msg, clean_op)             \
  if (!(condition))                                                            \
  {                                                                            \
    perror((msg));                                                             \
    (clean_op);                                                                \
    exit(status);                                                              \
  }

#define RETURN_IF_BAD_PERROR(condition, status, msg)                           \
  do                                                                           \
  {                                                                            \
    if (!(condition))                                                          \
    {                                                                          \
      perror((msg));                                                           \
      return (status);                                                         \
    }                                                                          \
  } while (0)

#define RETURN_IF_BAD_PERROR_CLEAN(condition, status, msg, clean_op)           \
  if (!(condition))                                                            \
  {                                                                            \
    perror((msg));                                                             \
    (clean_op);                                                                \
    return (status);                                                           \
  }

#define PANIC_IF_BAD(cond, error_msg)                                          \
  do                                                                           \
  {                                                                            \
    if (!(cond))                                                               \
    {                                                                          \
      fprintf(stderr, "[%s %d] - " error_msg "\n", __FILE__, __LINE__);        \
      abort();                                                                 \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define DEBUG_ONLY(expression)                                                 \
  do                                                                           \
  {                                                                            \
    expression;                                                                \
  } while (0);

#define DEBUG_BAD_MEM(ptr, type) DEBUG_ONLY(ptr = BAD_MEM(type))
#else
#define DEBUG_ONLY(expression)
#define DEBUG_BAD_MEM(ptr, type)
#endif

#define SUCCESS 0
#define FAIL 1

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#endif /*__UTILS_H__*/
