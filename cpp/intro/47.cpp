#include <iostream>
class A
{
public:
  A(int a) : m_a(a) {};
  virtual A* Clone() = 0;
  virtual void Print();

private:
  int m_a;
};
void A::Print()
{
  std::cout << "a = " << m_a << std::endl;
}

class B : public A
{
public:
  B(int a, int b) : A(a), m_b(b) {};
  virtual void Print();
  B* Clone();

private:
  int m_b;
};
B* B::Clone()
{
  return new B(*this);
}
void B::Print()
{
  A::Print();
  std::cout << "b = " << m_b << std::endl;
}

int main()
{
  A* a = new B(5, 7);
  A* copy = a->Clone();

  copy->Print();
  return 0;
}