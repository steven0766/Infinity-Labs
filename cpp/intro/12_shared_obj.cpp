#include <iostream>

extern "C"
{
  void AddPrint(int a, int b)
  {
    std::cout << a << " + " << b << " = " << a + b << std::endl;
  }

  void SubPrint(int a, int b)
  {
    std::cout << a << " - " << b << " = " << a - b << std::endl;
  }
}