#include <iostream>

void Foo();
void Foo(int i);
void Foo(char c);

int main()
{
  /*// exe a
  Foo();
  Foo(5);
  Foo('A');*/

  // exe c
  short a = 10;
  Foo(a); // foo(int)
  float f = 25.4;
  //   Foo(f); // foo(double)
  int* ptr1 = new int();
  //   Foo(ptr1); // invalid conversion from ‘int*’ to ‘char’ [-fpermissive]
  //   Foo(NULL); // call of overloaded ‘Foo(NULL)’ is ambiguous
  unsigned char uc = 'a';
  Foo(uc); // foo(int)
  //   Foo(NULL); // call of overloaded ‘Foo(NULL)’ is ambiguous

  return 0;
}

void Foo()
{
  std::cout << "foo()" << std::endl;
}

void Foo(int i)
{
  std::cout << "foo(int)" << std::endl;
}

void Foo(char c)
{
  std::cout << "Foo(char)" << c << std::endl;
}