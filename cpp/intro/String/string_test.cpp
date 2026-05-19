#include "string.hpp"
#include <cstring>
#include <iostream>
#include <sstream>

using namespace hrd42;

#define TEST(name, condition)                                                  \
  std::cout << (condition ? "PASS" : "FAIL") << " : " << name << "\n"

void TestCtor();
void TestCompare();
void TestAppend();
void TestAdd();
void TestIndex();
void TestLength();
void TestCstr();
void TestStream();
void TestLongerOrShorter();

int main()
{
  TestCtor();
  TestCompare();
  TestAppend();
  TestAdd();
  TestIndex();
  TestLength();
  TestCstr();
  TestStream();
  TestLongerOrShorter();

  return 0;
}

void TestCtor()
{
  String s1;
  String s2 = "123asd";
  String s3 = s2;
  TEST("default ctor", s1 == "");
  TEST("ctor with c-string", s2 == "123asd");
  TEST("copy ctor", s3 == s2);
}

void TestCompare()
{
  String s1 = "123";
  String s2 = "abd";
  String s3 = "abd";
  TEST("== on different strings", !(s1 == s2));
  TEST("!= on different strings", s1 != s2);
  TEST("== on equal strings", s2 == s3);
  TEST("!= on equal strings", !(s2 != s3));
}

void TestAppend()
{
  String s1 = "hello";
  String s2 = " world";
  s1 += s2;
  TEST("+= result", s1 == "hello world");
  TEST("+= doesn't change s2", s2 == " world");

  String s3 = "a";
  s3 += s3;
  TEST("+= self-append", s3 == "aa");
}

void TestAdd()
{
  String s1 = "foo";
  String s2 = "bar";
  String s3 = s1 + s2;
  TEST("+ result", s3 == "foobar");
  TEST("+ s1 unchanged", s1 == "foo");
  TEST("+ s2 unchanged", s2 == "bar");
}

void TestIndex()
{
  String s = "hello";
  TEST("[] read", s[0] == 'h');
  TEST("[] read last", s[4] == 'o');
  s[0] = 'H';
  TEST("[] write", s == "Hello");

  const String cs = "abc";
  TEST("[] const read", cs[1] == 'b');
}

void TestLength()
{
  String s1 = "";
  String s2 = "hello";
  String s3 = "hello world";
  TEST("length empty", s1.Length() == 0);
  TEST("length 5", s2.Length() == 5);
  TEST("length 11", s3.Length() == 11);
}

void TestCstr()
{
  String s = "test";
  TEST("Cstr returns correct", 0 == strcmp(s.Cstr(), "test"));
}

void TestStream()
{
  // operator
  std::ostringstream oss;
  String s = "hello";
  oss << s;
  TEST("<< output", oss.str() == "hello");

  // operator>>
  std::istringstream iss("world next");
  String t;
  iss >> t;
  TEST(">> read word", t == "world");

  String t2;
  iss >> t2;
  TEST(">> read second word", t2 == "next");

  String long_word;
  for (int i = 0; i < 200; ++i)
    long_word += "x";

  std::istringstream iss2(long_word.Cstr());
  String t3;
  iss2 >> t3;
  TEST(">> long word (>128 chars)", t3 == long_word);
}

void TestLongerOrShorter()
{
  String s1 = "123";
  String s2 = "1234";
  String s3 = "123";
  TEST("is s1 longer then s2? ", 0 == (s1 > s2));
  TEST("is s2 longer then s1? ", 1 == (s2 > s1));
  TEST("is s1 longer then s3? ", 0 == (s1 > s3));
  TEST("is s1 shorter then s2? ", 1 == (s1 < s2));
  TEST("is s2 shorter then s1? ", 0 == (s2 < s1));
  TEST("is s1 shorter then s3? ", 0 == (s1 < s3));
}