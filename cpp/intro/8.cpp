#include <iostream>

int main()
{
  std::cout << "hello" << " world" << std::endl; // simple print
  int i = 5;
  std::cout << "theree is " << i << "humshe torrah" << std::endl;
  std::cin >> i; // get number from user
  std::cout << "wrote number: " << i << std::endl;
  std::cerr << "ahamdulilah" << i; // print even if program crashe
}