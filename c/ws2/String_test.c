/************************************
 * Reviewer: Daria                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ************************************/

#include <stdio.h> /* printf */
#include <string.h> /* char x[] */
#include "String.h"

static void TestStrLen();
static void TestStrCmp();

int main()
{
  TestStrLen();
  TestStrCmp();
  return 0;
}

/* check StrLen function from String.c */
static void TestStrLen()
{
  char str[] = "aaa";
  char str2[] = "12345";
  if (StrLen(str) != strlen(str))
  {
    printf("Error:\n"
    "StrLen: %lu, strlen: %lu\n", StrLen(str), strlen(str));
    return;
  }
  if (StrLen(str2) != strlen(str2))
  {
    printf("Error:\n"
    "StrLen: %lu, strlen: %lu\n", StrLen(str2), strlen(str2));
    return;
  }
  
  printf("Test success: StrLen\n");
}

/* check StrCmp functino from String.c */
static void TestStrCmp()
{
    char str_cmp1[] = "abcde", str_cmp2[] = "edcba";
    char str1_cmp1[] = "aaaaa", str1_cmp2[] = "aaaaa";
    
    if ((StrCmp(str_cmp1, str_cmp2) >= 0) &&
       (strcmp(str_cmp1, str_cmp2) < 0))
    {
      printf("Test failed:\n"
      "StrCmp: %d, strcmp: %d", StrCmp(str_cmp1, str_cmp2), 
                                strcmp(str_cmp1, str_cmp2));
      return;
    }
    if ((StrCmp(str1_cmp1, str1_cmp2) != 0) && 
       (strcmp(str1_cmp1, str1_cmp2) == 0))
    {
      printf("Test failed:\n"
      "StrCmp: %d, strcmp: %d", StrCmp(str1_cmp1, str1_cmp2), 
                                strcmp(str1_cmp1, str1_cmp2));
      return;
    }
    printf("Test success: StrCmp\n");
}
