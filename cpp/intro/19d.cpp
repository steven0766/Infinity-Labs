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

  std::cout << "\nFoo(i)" << std::endl;
  Foo(i);

  std::cout << "\nFoo(reinterpret_cast)" << std::endl;
  Foo(reinterpret_cast<const double&>(i));

  std::cout << "\nBar(reinterpret_cast)" << std::endl;
  Bar(reinterpret_cast<double&>(i));

  return 0;
}