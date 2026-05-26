#include "line.hpp"
#include "glut_utils.h"
#include "point.hpp"
#include <cmath>

using namespace hrd42;
using namespace ilrd;

Line::Line(const ilrd::Point& center, double length, double angle, COLORS color)
    : Shape(center, angle, color), m_length(length)
{
}

Line& Line::Draw()
{
  double half_length = m_length / 2;
  double angle = GetAngle();
  ilrd::Point center = GetCenter();

  ilrd::Point p1(center.GetX() - half_length, center.GetY());
  ilrd::Point p2(center.GetX() + half_length, center.GetY());

  p1.Revolve(center, angle);
  p2.Revolve(center, angle);

  DrawPolygon(GetColor(), 2, (int)p1.GetX(), (int)p1.GetY(), (int)p2.GetX(),
              (int)p2.GetY());

  return *this;
}

Line* Line::Clone() const
{
  return new Line(GetCenter(), m_length, GetAngle(), GetColor());
}