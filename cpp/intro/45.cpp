#include <iostream>

class A
{
public:
  //   virtual void foo() = 0;
  virtual ~A() = 0;

private:
  int m_a;
};
A::~A()
{
}

class B : public A
{
public:
  //   ~B()
  //   {
  //   }
  //   void foo();

private:
  int m_b;
};

// void A::foo()
// {
//   std::cout << "I am in A object" << std::endl;
// }

// void B::foo()
// {
//   std::cout << "I am in B object" << std::endl;
//   A::foo();
// }

int main()
{
  B b;
  //   b.foo();

  return 0;
}