#include "slist.hpp"
#include <iostream>

using namespace hrd42;

int main()
{
  int a[5] = {1, 2, 3, 4, 5};
  size_t arr_size = sizeof(a) / sizeof(a[0]);
  size_t size = 0;
  Slist* list = new Slist();
  size_t i = 0;
  int var = 0;

  size = list->Size();

  if (0 != size)
  {
    std::cout << "size of list is: " << size << std::endl;
  }
  if (!list->IsEmpty())
  {
    std::cout << "list isn't empty" << std::endl;
  }

  for (i = 0; i < arr_size; ++i)
  {
    list->PushFront(a[i]);
  }

  size = list->Size();
  if (arr_size != size)
  {
    std::cout << "wrong size: " << size << std::endl;
  }
  if (list->IsEmpty())
  {
    std::cout << "list is empty" << std::endl;
  }

  for (i = 0; i < size; ++i)
  {
    var = list->PopFront();
    if (a[arr_size - i - 1] != var)
    {
      std::cout << "expected to pop: " << a[arr_size - i - 1]
                << ", actual got: " << var << std::endl;
    }
  }

  delete list;

  return 0;
}