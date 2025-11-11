/************************************
 * Reviewer: Daria                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ************************************/

#include "String.h"
#include <assert.h>

size_t StrLen(const char *s)
{
  size_t cnt = 0;
  assert(NULL != s);
  for (cnt = 0; *s != '\0'; cnt++)
    s++;
  return cnt;
}

int StrCmp(const char *s1, const char *s2)
{
  assert(NULL != s1);
  assert(NULL != s2);
  while ((*s1 != '\0') && (*s2 != '\0') && (s1 == s2))
  {
    ++s1;
    ++s2;
  }
  return (*s1 - *s2);
}
