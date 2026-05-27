#include <stdexcept>

void foo()
{
  throw std::runtime_error("something went wrong");
}

int main()
{
  foo();
  return 0;
}