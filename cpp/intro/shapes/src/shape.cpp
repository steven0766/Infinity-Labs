#include "shape.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Shape::Shape(const ilrd::Point& center, double angle, COLORS color)
    : m_center(center), m_angle(angle), m_color(color)
{
}
Shape::~Shape()
{
}

Shape& Shape::Move(const ilrd::Point& dest)
{
  this->m_center.Move(dest.GetX(), dest.GetY());
  return *this;
}
Shape& Shape::Move(double x_offest, double y_offset)
{
  Step(&(this->m_center), x_offest, y_offset);
  return *this;
}
Shape& Shape::Rotate(double angle)
{
  this->m_center.Revolve(this->m_center, angle);
  return *this;
}
Shape& Shape::Revolve(const ilrd::Point& pivot, double angle)
{
  this->m_center.Revolve(pivot, angle);
  return *this;
}
Shape& Shape::SetColor(COLORS color)
{
  this->m_color = color;
  return *this;
}
const ilrd::Point& Shape::GetCenter() const
{
  return this->m_center;
}
double Shape::GetAngle() const
{
  return this->m_angle;
}
COLORS Shape::GetColor() const
{
  return this->m_color;
}