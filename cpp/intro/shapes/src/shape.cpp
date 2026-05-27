/************************************************************
Description: shape
Developer: Steve
Reviewer: Itay
Version: 1
************************************************************/

#include "shape.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Shape::Shape(const Point& center, double angle, COLORS color)
    : m_center(center), m_angle(angle), m_color(color)
{
}
Shape::~Shape()
{
}

Shape& Shape::Move(const Point& dest)
{
  m_center.Move(dest.GetX(), dest.GetY());
  return *this;
}
Shape& Shape::Move(double x_offest, double y_offset)
{
  Step(&(m_center), x_offest, y_offset);
  return *this;
}
Shape& Shape::Rotate(double angle)
{
  m_angle += angle;
  return *this;
}
Shape& Shape::Revolve(const Point& pivot, double angle)
{
  m_center.Revolve(pivot, angle);
  m_angle += angle;
  return *this;
}
Shape& Shape::SetColor(COLORS color)
{
  m_color = color;
  return *this;
}
const Point& Shape::GetCenter() const
{
  return m_center;
}
double Shape::GetAngle() const
{
  return m_angle;
}
COLORS Shape::GetColor() const
{
  return m_color;
}