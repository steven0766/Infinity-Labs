#include <cstdlib>
#include <iostream>
#include <new>

int main()
{
  float* f = new float(12.6);
  delete f;

  f = new float[15];
  delete[] f;

  int* ptr = (int*)malloc(1000 * sizeof(int));
  std::cout << "allocated with malloc: " << ptr << std::endl;
  delete ptr;
  std::cout << "free malloc with delete " << std::endl;

  int* ptr2 = new int[1000];
  std::cout << "Allocated with new: " << ptr2 << std::endl;
  free(ptr2);
  std::cout << "free new with free " << std::endl;

  int* ptr3 = new (std::nothrow) int[100];
  free(ptr3);
  std::cout << "free new with flag and free " << std::endl;

  return 0;
}