/************************************
 * Reviewer: shalev                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ************************************/

#ifndef __STRING_H__
#define __STRING_H__
  
#include <stddef.h>

/* check the length of the string */
size_t StrLen(const char* s);

/* compare 2 strings if they are equal or not */
int StrCmp(const char* s1, const char* s2);

/* copy string from src to dst */
char *StrCpy(char * dst, const char * src);

/* copy first dsize chars from src to dst */
char* StrNCpy(char* dst, const char* src, size_t dsize);

/* compare 2 strings till n character */
int StrNCmp(const char* s1, const char* s2, size_t n);

/* compare 2 strings ignoring case sensitivity */
int StrCaseCmp(const char *s1, const char *s2);

/* find the first letter((char)c) in string(s) */
char* StrChr(const char *s, int c);

/* duplicate s string to new 1 and return it */
char* StrDup(const char *s);

/* append src to the end of dst */
char* StrCat(char* dst, const char* src);

/* append n chars from src to the end of dst */
char* StrNCat(char* dst, const char* src, size_t n);

/* find needle in s1 */
char* StrStr(const char *s1, const char *needle);

/* counting first letters from accept on s, count them till the first letter that doesn't have in accept */
size_t StrSpn(const char *s, const char *accept);

/* remove delimeters from str */
char* StrTok(char* str, const char* delim);

/* 7 boom game */
void SevenBoom(int start, int end);

/* remove spaces or tabs from beginnind, end and double spaces\ tabs in line */
void RemoveSpace(char* str);

/* */
#endif /* __STRING_H__ */
