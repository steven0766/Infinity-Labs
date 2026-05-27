#include <iostream>
#include <stdexcept>

int Foo(int a);
void Bar();

class X
{
public:
  X();
  ~X();

private:
  int m_a;
};

X::X() : m_a(5)
{
}
X::~X()
{
  std::cerr << Foo(m_a) << std::endl;
}

int Foo(int a)
{
  throw std::bad_alloc();

  return 1;
}

void Bar(int a)
{
  throw std::bad_alloc();
}

void Fishi()
{
  X x1;

  Bar();
}

int main()
{
  try
  {
    Fishi();
  }
  catch (std::bad_alloc& ba)
  {
    std::cerr << ba.what() << '\n';
  }

  return 0;
}