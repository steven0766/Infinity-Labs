#include <iostream>

static int cnt = 0;

struct X
{
  //   explicit X();
  X(int a_, int b_ = 8);
  ~X();
  //   X(const X& other_);
  //   X& operator=(const X& other_);

  int m_a;
  const int m_b;
};

struct Y
{
  X m_x;
  int m_i;
};

// X::X() : m_a(3), m_b(4)
// {
//   // m_a = 3;
//   // m_b = 4;
//   std::cout << "this: " << this << " X  default ctor. m_a: " << m_a
//             << " m_b: " << m_b << std::endl;
//   ++cnt;
//   std::cout << " Ctor num: " << cnt << std::endl;
// }

X::X(int a_, int b_) : m_a(a_), m_b(b_)
{
  std::cout << "this: " << this << " X int ctor. m_a: " << m_a
            << " m_b: " << m_b << std::endl;
}

// X::X(const X& other_) : m_a(other_.m_a), m_b(other_.m_b)
// {
//   std::cout << "this: " << this << " X copy ctor. m_a: " << m_a
//             << " m_b: " << m_b << std::endl;
// }

// X& X::operator=(const X& other_)
// {
//   m_a = other_.m_a;
//   // m_b = other_.m_b;
//   std::cout << "this: " << this << " X assignment operator. m_a: " << m_a
//             << " does not change m_b: " << m_b << std::endl;
//   return *this;
// }

X::~X()
{
  std::cout << "this: " << this << " X Dtor. m_a: " << m_a << " m_b: " << m_b
            << std::endl;
  std::cout << "Ctor end num: " << cnt << std::endl;
  --cnt;
}

int main()
{
  X x1;
  //   X x2(7.2);
  //   X* px = new X(x2);
  X x3(9, 10);
  //   X x4(x1);
  //   double* xp = new double[10];

  //   Y y1;
  //   std::cout << "y.m_i: " << y1.m_i << std::endl;
  //   y1.m_x.m_a = 250;
  //   Y y2(y1);
  //   Y y3;

  //   y3 = y1;

  x1 = x3;

  //   std::cout << "size of struct: " << sizeof(x1) << std::endl;

  //   delete px;
  //   px = 0;
  //   delete xp;

  //   X x1;
  //   X x2(x1);
  Y y1 = {{2, 4}, 5};
  // X x3(128);
  // X x4 = x3;
  // X x5(100, 200);

  return 0;
}