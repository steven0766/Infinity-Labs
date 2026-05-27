#include "circle.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Circle::Circle(const Point& center, double radius, double angle, COLORS color)
    : Shape(center, angle, color), m_radius(radius)
{
}

Circle& Circle::Draw()
{
  DrawCircle(GetColor(), GetCenter().GetX(), GetCenter().GetY(), m_radius);

  return *this;
}

Circle* Circle::Clone() const
{
  return new Circle(GetCenter(), m_radius, GetAngle(), GetColor());
}