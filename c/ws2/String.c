/************************************
 * Reviewer: shalev                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ***********************************/

#include "String.h"
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

size_t StrLen(const char *s)
{
  size_t cnt = 0;
  
  assert(NULL != s);
  
  for (cnt = 0; '\0' != *s; cnt++)
  {
    ++s;
  }
  return cnt;
}

int StrCmp(const char *s1, const char *s2)
{
  assert(NULL != s1);
  assert(NULL != s2);
  
  while (('\0' != *s1) && ('\0' != *s2) && (*s1 == *s2))
  {
    ++s1;
    ++s2;
  }
  
  return (*s1 - *s2);
}

char* StrCpy(char* dst, const char* src)
{
  char* p = dst;
  
  assert(NULL != dst);
  assert(NULL != src);
  
  while ('\0' != *src)
  {
    *p = *src;
    ++p;
    ++src;
  }
  *p = '\0';

  return dst;
}

char* StrNCpy(char* dst, const char* src, size_t dsize)
{
  char* p = dst;

  assert(NULL != dst);
  assert(NULL != src);
  
  while (0 != dsize && '\0' != *src)
  {
    *p = *src;
    ++p;
    ++src;
    --dsize;
  }
  
  while (0 != dsize)
  {
    *p = '\0';
    --dsize;
    ++p;
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
  int c1, c2;
  
  assert(NULL != s1);
  assert(NULL != s2);
  
  while (('\0' != *s1) && ('\0' != *s2))
  {
    c1 = tolower((unsigned char)*s1);
    c2 = tolower((unsigned char)*s2);
    
    if (c1 != c2)
    {
      return c1 - c2;
    }
    ++s1;
    ++s2;
  }
  return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

char* StrChr(const char *s, int c)
{
  assert(NULL != s);
  
  while ('\0' != *s)
  {
    if(*s == (char)c)
    {
      return (char*)s;
    }
    ++s;
  }
  
  if('\0' == (char)c)
  {
    return (char*)s;
  }
  
  return NULL;
}

char* StrDup(const char *s)
{
  size_t s_size;
  char* start_of_s;
  char* dup;
  
  assert(NULL != s);
  
  s_size = strlen(s) + 1;
  dup = (char*)malloc(s_size * sizeof(char));
  
  if (NULL == dup)
  {
    return NULL;
  }
  
  start_of_s = dup;
  
  while ('\0' != *s)
  {
    *dup = *s;
    ++dup;
    ++s;
  }
  
  *dup = '\0';
  /* free dup in ptr I will build in test_func */
  return start_of_s;
}

char* StrCat(char* dst, const char* src)
{
  char* start_of_dst = dst;

  assert(NULL != dst);
  assert(NULL != src);
  
  while ('\0' != *dst)
  {
    ++dst;
  }
  
  while ('\0' != *src)
  {
    *dst = *src;
    ++src;
    ++dst;
  }
  
  *dst = '\0';
  
  return start_of_dst; 
}

char* StrNCat(char* dst, const char* src, size_t n)
{
  char* start_of_dst = dst;

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
  
  return start_of_dst;
}

char* StrStr(const char *s1, const char *needle)
{
  const char* needle_sus;
  const char* check_letters;
  
  assert(NULL != s1);
  assert(NULL != needle);
  
  if ('\0' == *needle)
  {
    return (char*)s1;
  }
  
  while ('\0' != *s1)
  {
    check_letters = needle;
    
    if (*s1 == *check_letters)
    {
      needle_sus = s1;
      
      while ((*needle_sus == *check_letters) && ('\0' != *check_letters))
      {
        ++check_letters;
        ++needle_sus;
      }
      
      if ('\0' == *check_letters)
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
  const char* a;
  
  assert(NULL != s);
  assert(NULL != accept);
  
  while ('\0' != *s)
  {
    a = accept;
    
    while ('\0' != *a && *a != *s)
    {
      ++a;
    }
    
    if ('\0' == *a)
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
    char* start;

    assert(NULL != delim);

    if (str != NULL)
    {
        next = str;
    }

    if (next == NULL)
    {
        return NULL;
    }

    while (*next != '\0' && StrChr(delim, *next) != NULL)
    {
        ++next;
    }

    if (*next == '\0')
    {
        next = NULL;
        return NULL;
    }

    start = next;

    while (*next != '\0' && StrChr(delim, *next) == NULL)
    {
        ++next;
    }

    if (*next != '\0')
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

int Palindrome(const char* str)
{
  const char* left = str;
  const char* right = str;
  
  assert(NULL != str);
  
  while ('\0' != *right)
  {
    ++right;
  }
  
  --right;
  
  while (right > left)
  {
    if(*right != *left)
    {
      return 0;
    }
    
    --right;
    ++left;
  }
  
  return 1;
}

static int ContainSeven(int num)
{
  if (num < 0)
  {
    num = -num;
  }
  
  while (num > 0)
  {
    if (7 == num % 10)
    {
      return 1;
    }
    
    num/=10;
  }
  
  return 0;
}

void SevenBoom(int start, int end)
{
  int i = 0;
  
  for (i = start; i < end; ++i)
  {
    if (0 == i % 7 || ContainSeven(i))
    {
      printf("BOOM\n");
    }
    else
    {
      printf("%d", i);
    }
  }
}

static int IsSpaceOrTab(char c)
{
  return (' ' == c || '\t' == c);
}

void RemoveSpace(char* str)
{
  char* read = str;
  char* corrected_str = str;
  
  int space_seq = 0;
  
  while  ('\0' != *read && IsSpaceOrTab(*read))
  {
    ++read;
  }
  
  while ('\0' != *read)
  {
    if (IsSpaceOrTab(*read))
    {
      if (0 == space_seq)
      {
        space_seq = 1;
        *corrected_str = *read;
        ++corrected_str;
      }
    }
    else
    {
      *corrected_str = *read;
      space_seq = 0;
      ++corrected_str;
    }
    
    ++read;
  }
  
  while (corrected_str > str && IsSpaceOrTab(*(corrected_str - 1)))
  {
    --corrected_str;
  }
  
  *corrected_str = '\0';
}
