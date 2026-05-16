// test_complex.cpp
#include "complex.hpp"
#include <iostream>

using namespace hrd42;

#define TEST(name, condition)                                                  \
  std::cout << (condition ? "PASS" : "FAIL") << " : " << name << "\n"

int main()
{
  Complex c1;
  Complex c2(3);
  Complex c3(3, 4);

  TEST("default ctor", c1.GetReal() == 0 && c1.GetImg() == 0);
  TEST("single arg", c2.GetReal() == 3 && c2.GetImg() == 0);
  TEST("full ctor", c3.GetReal() == 3 && c3.GetImg() == 4);

  // comparing
  TEST("== true", Complex(3, 4) == Complex(3, 4));
  TEST("== false", !(Complex(3, 4) == Complex(1, 2)));
  TEST("!= true", Complex(3, 4) != Complex(1, 2));
  TEST("!= false", !(Complex(3, 4) != Complex(3, 4)));

  // += -= *= /=
  Complex add(1, 2);
  add += Complex(3, 4);
  TEST("+=", add == Complex(4, 6));

  Complex sub(5, 6);
  sub -= Complex(3, 4);
  TEST("-=", sub == Complex(2, 2));

  Complex mul(1, 2);
  mul *= Complex(3, 4);
  // (1+2i)*(3+4i) = -5+10i
  TEST("*=", mul == Complex(-5, 10));

  Complex div(3, 4);
  div /= Complex(1, 2);
  // (3+4i)/(1+2i) = 2.2 - 0.4i
  TEST("/=", div == Complex(2.2, -0.4));

  // + - * /
  Complex p(1, 2);
  Complex q(3, 4);

  TEST("operator+", p + q == Complex(4, 6));
  TEST("operator-", p - q == Complex(-2, -2));
  TEST("operator*", p * q == Complex(-5, 10));
  TEST("no side effect on +", p == Complex(1, 2));

  // implicit conversion
  TEST("implicit right", Complex(1, 2) + 5.0 == Complex(6, 2));
  TEST("implicit left", 5.0 + Complex(1, 2) == Complex(6, 2));

  // operator
  std::cout << "operator<< : " << Complex(3, 4) << "\n";
  std::cout << "operator<< : " << Complex(1.5, 2.5) << "\n";

  // operator>>
  Complex in1;
  std::cout << "enter (7,9) : ";
  std::cin >> in1;
  TEST(">> simple", in1 == Complex(7, 9));

  Complex in2;
  std::cout << "enter (70,132) : ";
  std::cin >> in2;
  TEST(">> multi digit", in2 == Complex(70, 132));

  Complex in3;
  std::cout << "enter (1.5,2.5) : ";
  std::cin >> in3;
  TEST(">> double", in3 == Complex(1.5, 2.5));

  return 0;
}