#include "slist_generic.hpp"
#include <iostream>

using namespace hrd42;

struct Person
{
  const size_t id;
  size_t age;
  const char* name;
};

int main()
{
  Person p[3] = {{123456789, 20, "steve"},
                 {987654321, 25, "shalev"},
                 {678954321, 22, "nemo"}};

  Slist<Person>* list = new Slist<Person>();
  int i = 0;

  if (0 != list->Size())
  {
    std::cout << "list not empty: " << list->Size() << " nodes" << std::endl;
  }
  if (true != list->IsEmpty())
  {
    std::cout << "IsEmpty func doen't implemented right" << std::endl;
  }

  for (i = 0; i < 3; ++i)
  {
    list->PushFront(p[i]);
  }

  if (3 != list->Size())
  {
    std::cout << "list node count isn't right: " << list->Size() << " nodes"
              << std::endl;
  }
  if (false != list->IsEmpty())
  {
    std::cout << "IsEmpty func doen't implemented right" << std::endl;
  }

  for (i = 0; i < 3; ++i)
  {
    list->PopFront();
  }

  if (0 != list->Size())
  {
    std::cout << "list not empty: " << list->Size() << " nodes" << std::endl;
  }
  return 0;
}