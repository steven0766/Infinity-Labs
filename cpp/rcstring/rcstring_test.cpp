#include "rcstring.hpp"
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
void TestRC();

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
  TestRC();

  return 0;
}

void TestCtor()
{
  RCString s1;
  RCString s2 = "123asd";
  RCString s3 = s2;
  TEST("default ctor", s1 == "");
  TEST("ctor with c-string", s2 == "123asd");
  TEST("copy ctor", s3 == s2);
}

void TestCompare()
{
  RCString s1 = "123";
  RCString s2 = "abd";
  RCString s3 = "abd";
  TEST("== on different strings", !(s1 == s2));
  TEST("!= on different strings", s1 != s2);
  TEST("== on equal strings", s2 == s3);
  TEST("!= on equal strings", !(s2 != s3));
}

void TestAppend()
{
  RCString s1 = "hello";
  RCString s2 = " world";
  s1 += s2;
  TEST("+= result", s1 == "hello world");
  TEST("+= doesn't change s2", s2 == " world");

  RCString s3 = "a";
  s3 += s3;
  TEST("+= self-append", s3 == "aa");
}

void TestAdd()
{
  RCString s1 = "foo";
  RCString s2 = "bar";
  RCString s3 = s1 + s2;
  TEST("+ result", s3 == "foobar");
  TEST("+ s1 unchanged", s1 == "foo");
  TEST("+ s2 unchanged", s2 == "bar");
}

void TestIndex()
{
  RCString s = "hello";
  TEST("[] read", s[0] == 'h');
  TEST("[] read last", s[4] == 'o');
  s[0] = 'H';
  TEST("[] write", s == "Hello");

  const RCString cs = "abc";
  TEST("[] const read", cs[1] == 'b');
}

void TestLength()
{
  RCString s1 = "";
  RCString s2 = "hello";
  RCString s3 = "hello world";
  TEST("length empty", s1.Length() == 0);
  TEST("length 5", s2.Length() == 5);
  TEST("length 11", s3.Length() == 11);
}

void TestCstr()
{
  RCString s = "test";
  TEST("Cstr returns correct", 0 == strcmp(s.Cstr(), "test"));
}

void TestStream()
{
  std::ostringstream oss;
  RCString s = "hello";
  oss << s;
  TEST("<< output", oss.str() == "hello");

  std::istringstream iss("world next");
  RCString t;
  iss >> t;
  TEST(">> read word", t == "world");
  RCString t2;
  iss >> t2;
  TEST(">> read second word", t2 == "next");

  RCString long_word;
  for (int i = 0; i < 200; ++i)
  {
    long_word += "x";
  }
  std::istringstream iss2(long_word.Cstr());
  RCString t3;
  iss2 >> t3;
  TEST(">> long word (>128 chars)", t3 == long_word);
}

void TestLongerOrShorter()
{
  RCString s1 = "123";
  RCString s2 = "1234";
  RCString s3 = "123";
  TEST("is s1 longer then s2? ", 0 == (s1 > s2));
  TEST("is s2 longer then s1? ", 1 == (s2 > s1));
  TEST("is s1 longer then s3? ", 0 == (s1 > s3));
  TEST("is s1 shorter then s2? ", 1 == (s1 < s2));
  TEST("is s2 shorter then s1? ", 0 == (s2 < s1));
  TEST("is s1 shorter then s3? ", 0 == (s1 < s3));
}

void TestRC()
{
  RCString s1 = "hello";
  RCString s2 = s1;
  RCString s3 = s2;

  TEST("shared after copy ctor", s1.Cstr() == s2.Cstr());
  TEST("shared after second copy", s1.Cstr() == s3.Cstr());

  s2[0] = 'H';
  TEST("CoW on write — s2 changed", s2 == "Hello");
  TEST("CoW on write — s1 unchanged", s1 == "hello");
  TEST("CoW on write — s1/s3 still shared", s1.Cstr() == s3.Cstr());
  TEST("CoW on write — s2 detached", s1.Cstr() != s2.Cstr());

  RCString s4 = "hello";
  RCString s5 = s4;
  s4 += " world";
  TEST("+= detaches", s4 == "hello world");
  TEST("+= s5 unchanged", s5 == "hello");
  TEST("+= s4/s5 not shared", s4.Cstr() != s5.Cstr());
}