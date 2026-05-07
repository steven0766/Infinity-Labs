#include <iostream>

extern "C"
{
  void AddPrint(int a, int b);
  void SubPrint(int a, int b);
}

int main()
{
  AddPrint(3, 4);
  SubPrint(10, 3);
  return 0;
}