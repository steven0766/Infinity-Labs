#include <iostream>

void Foo(const double& d)
{
  std::cout << "Foo address: " << &d << std::endl;
  std::cout << "Foo value:   " << d << std::endl;
}

void Bar(double& d)
{
  std::cout << "Bar address: " << &d << std::endl;
  std::cout << "Bar value:   " << d << std::endl;
}

int main()
{
  int i = 5;
  std::cout << "i address: " << &i << std::endl;
  std::cout << "i value:   " << i << std::endl;

  // a. call with int
  Foo(i);
  // Bar(i);  //non-const ref cant bind to temporary

  // b. reinterpret_cast to force compile
  Foo(reinterpret_cast<const double&>(i));
  Bar(reinterpret_cast<double&>(i));

  return 0;
}