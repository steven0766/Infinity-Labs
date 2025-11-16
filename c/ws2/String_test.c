/************************************
 * Reviewer: shalev                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ***********************************/

#include <stdio.h>  /* printf         */
#include <string.h> /* strlen, strcmp, strcasecmp */
#include <stdlib.h> /* malloc         */

#include "String.h" /* StrLen, StrCmp */

#define ARRAYSIZE(X) sizeof(X) / sizeof(X[0])

static void TestStrLen();
static void TestStrCmp();
static void TestStrCpy(); 
static void TestStrNCpy();
static void TestStrNCmp();
static void TestStrCaseCmp();
static void TestStrChr();
static void TestStrDup();
static void TestStrCat();
static void TestStrNCat();
static void TestStrStr();
static void TestStrSpn();
static void TestStrTok();


int main()
{
  TestStrLen();
  TestStrCmp();
  TestStrCpy(); 
  TestStrNCpy();
  TestStrNCmp();
  TestStrCaseCmp();
  TestStrChr();
  TestStrDup();
  TestStrCat();
  TestStrNCat();
  TestStrStr();
  TestStrSpn();
  TestStrTok();
  
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

static void TestStrCpy()
{
    char* inputs[] = {"e12e1r", "res5w55wes5", "est5ews56testsrsetse6", "gh", "", " ", "\0", "\t"};
    char dst_test[80] = {'\0'};
    char dst_strcpy[80] = {'\0'};
    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(inputs); ++i)
    {
        memset(dst_test, 0, sizeof(dst_test));
        memset(dst_strcpy, 0, sizeof(dst_strcpy));

        StrCpy(dst_test, inputs[i]);
        strcpy(dst_strcpy, inputs[i]);

        if (0 != strcmp(dst_test, dst_strcpy))
        {
            printf("Error: %s\n", inputs[i]);
            return;
        }
    }

    printf("Test Success: StrCpy\n");
}

static void TestStrNCpy()
{
    char* inputs[] = {"349acn", "932hrf8q7dn", "23ync", "", " ", "\0", "\t"};
    size_t n_cpy[] = {4, 7, 3, 2, 1, 2, 2};
    char dst_test[80] = {'\0'};
    char dst_strncpy[80] = {'\0'};
    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(inputs); i++)
    {
        memset(dst_test, 0, sizeof(dst_test));
        memset(dst_strncpy, 0, sizeof(dst_strncpy));

        StrNCpy(dst_test, inputs[i], n_cpy[i]);
        strncpy(dst_strncpy, inputs[i], n_cpy[i]);

        if (0 != strcmp(dst_test, dst_strncpy))
        {
            printf("Error:\n StrNCpy: %s\n strncpy: %s\n",
                    dst_test, dst_strncpy);
            return;
        }
    }

    printf("Test Success: StrNCpy\n");
}

static void TestStrNCmp()
{
    char* s1[] = {"abcdef", "aaaaaa", "abc", "xyz", ""};
    char* s2[] = {"abczzz", "aaaaaa", "abd", "xy", ""};
    size_t n[] = {3, 6, 2, 2, 1};
    int my_func, sys_func;
    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(s1); ++i)
    {
        my_func = StrNCmp(s1[i], s2[i], n[i]);
        sys_func = strncmp(s1[i], s2[i], n[i]);

        if ((0 > my_func && 0 <= sys_func) ||
            (0 < my_func && 0 >= sys_func) ||
            (0 == my_func && 0 != sys_func))
        {
            printf("Error:\n"
                   "StrNCmp(%s,%s,%lu) = %d, strncmp = %d\n",
                   s1[i], s2[i], n[i], my_func, sys_func);
            return;
        }
    }

    printf("Test Success: StrNCmp\n");
}

static void TestStrCaseCmp()
{
    char* a[] = {"AbCd", "abc", "HELLO", "zzz", "AaA"};
    char* b[] = {"aBcD", "AbC", "hello", "Zzz", "aaa"};
    int my_func, sys_func;
    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(a); ++i)
    {
        my_func = StrCaseCmp(a[i], b[i]);
        sys_func = strcasecmp(a[i], b[i]);

        if ((my_func < 0 && sys_func >= 0) ||
            (my_func > 0 && sys_func <= 0) ||
            (my_func == 0 && sys_func != 0))
        {
            printf("Error: StrCaseCmp(%s,%s) = %d, strcasecmp = %d\n",
                   a[i], b[i], my_func, sys_func);
            return;
        }
    }

    printf("Test Success: StrCaseCmp\n");
}

static void TestStrChr()
{
    char s[] = "hello world";
    char tests[] = {'h', 'o', 'w', 'z', '\0'};

    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(tests); ++i)
    {
        if (StrChr(s, tests[i]) != strchr(s, tests[i]))
        {
            printf("Error: StrChr(%c)\n", tests[i]);
            return;
        }
    }

    printf("Test Success: StrChr\n");
}

static void TestStrDup()
{
    char* inputs[] = {"hello", "", " ", "\t", "abc123"};
    size_t i = 0;
    char* my_func;
    char* sys_func;

    for (i = 0; i < ARRAYSIZE(inputs); ++i)
    {
        my_func = StrDup(inputs[i]);
        sys_func = StrDup(inputs[i]);

        if (strcmp(my_func, sys_func) != 0)
        {
            printf("Error: StrDup(%s): %s vs %s\n",
                   inputs[i], my_func, sys_func);
            free(my_func);
            free(sys_func);
            return;
        }

        free(my_func);
        free(sys_func);
    }

    printf("Test Success: StrDup\n");
}

static void TestStrCat()
{
    char* inputs[] = {"123", "xyz", "", " ", "\t"};
    size_t i = 0;
    char d1[100], d2[100];

    for (i = 0; i < ARRAYSIZE(inputs); ++i)
    {
        strcpy(d1, "abc");
        strcpy(d2, "abc");

        StrCat(d1, inputs[i]);
        strcat(d2, inputs[i]);

        if (strcmp(d1, d2) != 0)
        {
            printf("Error: StrCat: %s vs %s\n", d1, d2);
            return;
        }
    }

    printf("Test Success: StrCat\n");
}

static void TestStrNCat()
{
    char* inputs[] = {"123456", "xyz", "", " ", "\t"};
    size_t n[] = {3, 2, 1, 1, 1};
    char d1[100], d2[100];
    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(inputs); ++i)
    {
        strcpy(d1, "abc");
        strcpy(d2, "abc");

        StrNCat(d1, inputs[i], n[i]);
        strncat(d2, inputs[i], n[i]);

        if (0 != strcmp(d1, d2))
        {
            printf("Error: StrNCat(%s,%lu): %s vs %s\n",
                   inputs[i], n[i], d1, d2);
            return;
        }
    }

    printf("Test Success: StrNCat\n");
}

static void TestStrStr()
{
    char hay[] = "simple string test";
    char* needles[] = {"simple", "stuff", "test", "a", "nope", ""};
    char* my_func;
    char* sys_func;
    size_t i;

    for (i = 0; i < ARRAYSIZE(needles); ++i)
    {
        my_func = StrStr(hay, needles[i]);
        sys_func = strstr(hay, needles[i]);

        if (my_func != sys_func)
        {
            printf("Error: StrStr(%s)\n", needles[i]);
            return;
        }
    }

    printf("Test Success: StrStr\n");
}

static void TestStrSpn()
{
    char* s[] = {"abcdef123", "123abc", "aaaaaa", "", "x"};
    char* acc[] = {"abcde", "123", "a", "", "xyz"};
    size_t i = 0;

    for (i = 0; i < ARRAYSIZE(s); ++i)
    {
        if (StrSpn(s[i], acc[i]) != strspn(s[i], acc[i]))
        {
            printf("Error: StrSpn(%s,%s)\n", s[i], acc[i]);
            return;
        }
    }

    printf("Test Success: StrSpn\n");
}

static void TestStrTok()
{
    char s1[] = "abc,def,ghi";
    char s2[] = "abc,def,ghi";

    char* tok1 = StrTok(s1, ",");
    char* tok2 = strtok(s2, ",");

    while (NULL != tok1 || NULL != tok2)
    {
        if ((NULL == tok1 && NULL != tok2) ||
            (NULL != tok1 && NULL == tok2) ||
            (NULL != tok1 && 0 != strcmp(tok1, tok2)))
        {
            printf("Error: strtok mismatch\n");
            return;
        }

        tok1 = StrTok(NULL, ",");
        tok2 = strtok(NULL, ",");
    }

    printf("Test Success: strtok\n");
}
