#include <iostream>

class X
{
public:
  X()
  {
    std::cout << "Ctor X: " << this << std::endl;
  }
  virtual ~X()
  {
    std::cout << "Dtor X: " << this << std::endl;
  }

private:
  int m_a;
};

class Y : public X
{
public:
  Y()
  {
    std::cout << "Ctor Y: " << this << std::endl;
  }
  ~Y()
  {
    std::cout << "Dtor Y: " << this << std::endl;
  }

private:
  int m_b;
};

void Foo(X x);

void Foo(X x)
{
  std::cout << "Foo: " << &x << std::endl;
}

int main()
{
  //   X* xp = new Y[5];
  Y y1;
  Foo(y1);

  //   delete[] xp;

  return 0;
}