/************************************************************
Description: Complex
Developer: Steve
Reviewer: Lior
Version: 1
************************************************************/
#ifndef __ILRD_HRD42_COMPLEX_HPP__
#define __ILRD_HRD42_COMPLEX_HPP__

#include <iostream>

namespace hrd42
{

class Complex;
// friend free func declarations
const Complex operator+(const Complex& lhs, const Complex& rhs);
const Complex operator-(const Complex& lhs, const Complex& rhs);
const Complex operator*(const Complex& lhs, const Complex& rhs);
const Complex operator/(const Complex& lhs, const Complex& rhs);

std::istream& operator>>(std::istream& is, Complex& complex);
std::ostream& operator<<(std::ostream& os, const Complex& complex);

// class declaration
class Complex
{
public:
  // non-explicit on purpose
  Complex(double real = 0, double img = 0);
  // rely on generated cctor, copy= and dtor

  double GetReal() const;
  void SetReal(double real);
  double GetImg() const;
  void SetImg(double img);

  bool operator!=(const Complex& other) const;
  bool operator==(const Complex& other) const;

  Complex& operator+=(const Complex& other);
  Complex& operator-=(const Complex& other);
  Complex& operator*=(const Complex& other);
  Complex& operator/=(const Complex& other);

private:
  double m_real;
  double m_img;

  // friend declarations
  friend const Complex operator+(const Complex& lhs, const Complex& rhs);
  friend const Complex operator-(const Complex& lhs, const Complex& rhs);
  friend const Complex operator*(const Complex& lhs, const Complex& rhs);
  friend const Complex operator/(const Complex& lhs, const Complex& rhs);

  friend std::istream& operator>>(std::istream& is, Complex& complex);
  friend std::ostream& operator<<(std::ostream& os, const Complex& complex);
};

inline Complex::Complex(double real_, double img_) : m_real(real_), m_img(img_)
{
}
inline double Complex::GetReal() const
{
  return m_real;
}
inline void Complex::SetReal(double real_)
{
  m_real = real_;
}
inline double Complex::GetImg() const
{
  return m_img;
}
inline void Complex::SetImg(double img_)
{
  m_img = img_;
}

inline bool Complex::operator!=(const Complex& other) const
{
  return !(*this == other);
}
inline bool Complex::operator==(const Complex& other) const
{
  return (m_real == other.m_real) && (m_img == other.m_img);
}

inline Complex& Complex::operator+=(const Complex& other)
{
  m_real += other.m_real;
  m_img += other.m_img;
  return *this;
}
inline Complex& Complex::operator-=(const Complex& other)
{
  m_real -= other.m_real;
  m_img -= other.m_img;
  return *this;
}
inline Complex& Complex::operator*=(const Complex& other)
{
  double new_real = (m_real * other.m_real - m_img * other.m_img);
  double new_img = (m_real * other.m_img + m_img * other.m_real);
  m_real = new_real;
  m_img = new_img;
  return *this;
}
inline Complex& Complex::operator/=(const Complex& other)
{
  double divide_by = (other.m_real * other.m_real + other.m_img * other.m_img);
  double new_real = (m_real * other.m_real + m_img * other.m_img) / divide_by;
  double new_img = (m_img * other.m_real - m_real * other.m_img) / divide_by;
  m_real = new_real;
  m_img = new_img;
  return *this;
}

inline const Complex operator+(const Complex& lhs, const Complex& rhs)
{
  Complex res(lhs);
  return (res += rhs);
}
inline const Complex operator-(const Complex& lhs, const Complex& rhs)
{
  Complex res(lhs);
  return (res -= rhs);
}
const Complex operator*(const Complex& lhs, const Complex& rhs)
{
  double new_real =
      (lhs.GetReal() * rhs.GetReal() - lhs.GetImg() * rhs.GetImg());
  double new_img =
      (lhs.GetReal() * rhs.GetImg() + lhs.GetImg() * rhs.GetReal());
  return Complex(new_real, new_img);
}
const Complex operator/(const Complex& lhs, const Complex& rhs)
{
  double divide_by =
      (rhs.GetReal() * rhs.GetReal() + lhs.GetImg() * lhs.GetImg());
  double new_real =
      (lhs.GetReal() * rhs.GetReal() + lhs.GetImg() * rhs.GetImg()) / divide_by;
  double new_img =
      (lhs.GetImg() * rhs.GetReal() - lhs.GetReal() * rhs.GetImg()) / divide_by;
  return Complex(new_real, new_img);
}
inline std::istream& operator>>(std::istream& is, Complex& complex)
{
  char trash;
  is >> trash >> complex.m_real >> trash >> complex.m_img >> trash;
  return is;
}
std::ostream& operator<<(std::ostream& os, const Complex& complex)
{
  os << '(' << complex.m_real << ',' << complex.m_img << ')';
  return os;
}

} // namespace hrd42
#endif // __ILRD_HRD42_COMPLEX_HPP__