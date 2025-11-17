/************************************
 * Reviewer: shalev                  *
 * Date: 11.11.25                   *
 * Version: 1                       *
 ************************************/

#ifndef __STRING_H__
#define __STRING_H__
  
#include <stddef.h>

/* return the length of the 's' string (incuding \0) */
size_t StrLen(const char* s);

/* compare 2 strings (s1, s2) 
   s1 > s2 will return positive number
   s1 < s2 will return negative number
   s1 = s2 will return 0 */
int StrCmp(const char* s1, const char* s2);

/* copy string from src to dst.
   return dst.
   note: dst must be large enough to
         contain all src letters. */
char *StrCpy(char * dst, const char * src);

/* copy first dsize chars from src to dst.
   return dst.
   note: dst must be large enough to 
   contain all letters. */
char* StrNCpy(char* dst, const char* src, size_t dsize);

/* compare 2 strings (s1, s2) till n number of character.
   s1 > s2 will return positive number.
   s1 < s2 will return negative number.
   s1 = s2 will return 0. */
int StrNCmp(const char* s1, const char* s2, size_t n);

/* compare 2 strings ignoring case sensitivity
   s1 > s2 will return positive number.
   s1 < s2 will return negative number.
   s1 = s2 will return 0.*/
int StrCaseCmp(const char *s1, const char *s2);

/* find the first letter((char)c) in string(s).
   if letter found, return pointer to the location.
   if not found, return NULL. */
char* StrChr(const char *s, int c);

/* duplicate s string to new 1 and return it.
   note: must free allocated variable. */
char* StrDup(const char *s);

/* append src to the end of dst.
   note: dst must be large enough to
   contain dst + src + 1 letters.*/
char* StrCat(char* dst, const char* src);

/* append n number of chars from src to the end of dst.
   note: dst must be large enough to
   contain dst + n + 1 letters. */
char* StrNCat(char* dst, const char* src, size_t n);

/* find needle in s1.
   needle = '\0' will return all s1 letters.
   if needle found in s1, will return all characters
   from where it was found in s1 till the end of s1.
   if not found return NULL.*/
char* StrStr(const char *s1, const char *needle);

/* counting first letters from accept on s, count them
   till the first letter that doesn't have in accept. */
size_t StrSpn(const char *s, const char *accept);

/* remove delimeters from str */
char* StrTok(char* str, const char* delim);

#endif /* __STRING_H__ */
