#include <iostream>

void foo(int t, int i = 3, float f = 4.0)
{
  std::cout << "t=" << t << " i=" << i << " f=" << f << std::endl;
}

int main()
{
  foo(3);
  foo(6, 5);
  foo(3, 7, 8.7);
  // foo(3, , 7.9);    //expecting expression for argument i
  return 0;
}