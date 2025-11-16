/************************************
 * Reviewer: shalev                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ***********************************/

#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "String.h"

size_t StrLen(const char *s)
{
  size_t cnt = 0;
  
  assert(NULL != s);
  
  for (cnt = 0; '\0' != *s; ++cnt)
  {
    ++s;
  }
  return cnt;
}

int StrCmp(const char *s1, const char *s2)
{
  assert(NULL != s1);
  assert(NULL != s2);
  
  while (('\0' != *s1) && (*s1 == *s2))
  {
    ++s1;
    ++s2;
  }
  
  return (*s1 - *s2);
}

char* StrCpy(char* dst, const char* src)
{
  char* dst_ptr = dst;
  
  assert(NULL != dst);
  assert(NULL != src);
  
  while ('\0' != *src)
  {
    *dst_ptr = *src;
    ++dst_ptr;
    ++src;
  }
  *dst_ptr = '\0';

  return dst;
}

char* StrNCpy(char* dst, const char* src, size_t dsize)
{
  char* dst_ptr = dst;

  assert(NULL != dst);
  assert(NULL != src);
  
  while (0 != dsize && '\0' != *src)
  {
    *dst_ptr = *src;
    ++dst_ptr;
    ++src;
    --dsize;
  }
  
  while (0 != dsize)
  {
    *dst_ptr = '\0';
    --dsize;
    ++dst_ptr;
  }
  
  return dst;
}

int StrNCmp(const char* s1, const char* s2, size_t n)
{
  size_t i = 0;
  const char* p_s1 = s1;
  const char* p_s2 = s2;
 
  assert(NULL != s1);
  assert(NULL != s2);
 
  for (i = 0; i < n; ++i)
  {
    if (*p_s1 != *p_s2)
    {
      return (*p_s1 - *p_s2);
    }
   
    if ('\0' == *p_s1)
    {
      break;
    }
   
    ++p_s1;
    ++p_s2;
  }
  return 0;
}

int StrCaseCmp(const char *s1, const char *s2)
{  
  assert(NULL != s1);
  assert(NULL != s2);
  
  while (('\0' != *s1) && (tolower(*s1) == tolower(*s2)))
  {
    ++s1;
    ++s2;
  }
  return tolower(*s1) - tolower(*s2);
}

char* StrChr(const char *s, int c)
{
  assert(NULL != s);
  
  while ('\0' != *s)
  {
    if(*s == c)
    {
      return (char*)s;
    }
    ++s;
  }
  
  if('\0' == c)
  {
    return (char*)s;
  }
  
  return NULL;
}

char* StrDup(const char *s)
{
  size_t s_size;
  char* s_ptr;
  char* dup;
  
  assert(NULL != s);
  
  s_size = StrLen(s) + 1;
  dup = (char*)malloc(s_size * sizeof(char));
  
  if (NULL == dup)
  {
    return NULL;
  }
  
  s_ptr = dup;
  
  StrCpy(s_ptr, s);

  return s_ptr;
}

char* StrCat(char* dst, const char* src)
{
  char* dst_ptr = dst;

  assert(NULL != dst);
  assert(NULL != src);
  
  while ('\0' != *dst)
  {
    ++dst;
  }
  
  StrCpy(dst, src);
  
  return dst_ptr; 
}

char* StrNCat(char* dst, const char* src, size_t n)
{
  char* dst_ptr = dst;

  assert(NULL != dst);
  assert(NULL != src);
  
  while ('\0' != *dst)
  {
    ++dst;
  }
  
  while (n > 0 && '\0' != *src)
  {
    *dst = *src;
    ++dst;
    ++src;
    --n;
  }
  
  *dst = '\0';
  
  return dst_ptr;
}

char* StrStr(const char *s1, const char *needle)
{
  const char* s1_ptr;
  const char* needle_ptr;
  
  assert(NULL != s1);
  assert(NULL != needle);
  
  if ('\0' == *needle)
  {
    return (char*)s1;
  }
  
  while ('\0' != *s1)
  {
    needle_ptr = needle;
    
    if (*s1 == *needle_ptr)
    {
      s1_ptr = s1;
      
      while ((*s1_ptr == *needle_ptr) && ('\0' != *needle_ptr))
      {
        ++needle_ptr;
        ++s1_ptr;
      }
      
      if ('\0' == *needle_ptr)
      {
        return (char*)s1;
      }
    }
    
    ++s1;
  }
  
  return NULL;
}

size_t StrSpn(const char *s, const char *accept)
{
  size_t cnt = 0;
  const char* accept_ptr;
  
  assert(NULL != s);
  assert(NULL != accept);
  
  while ('\0' != *s)
  {
    accept_ptr = accept;
    
    while ('\0' != *accept_ptr && *accept_ptr != *s)
    {
      ++accept_ptr;
    }
    
    if ('\0' == *accept_ptr)
    {
      return cnt;
    }
    
    ++cnt;
    ++s;
  }
  
  return cnt;
}

char* StrTok(char* str, const char* delim)
{
    static char* next = NULL;
    char* start = NULL;

    assert(NULL != delim);

    if (NULL != str)
    {
        next = str;
    }

    if (NULL == next)
    {
        return NULL;
    }

    while ('\0' != *next && NULL != StrChr(delim, *next))
    {
        ++next;
    }

    if ('\0' == *next)
    {
        next = NULL;
        return NULL;
    }

    start = next;

    while ('\0' != *next && NULL == StrChr(delim, *next))
    {
        ++next;
    }

    if ('\0' != *next)
    {
        *next = '\0';
        ++next;
    }
    else
    {
        next = NULL;
    }

    return start;
}
