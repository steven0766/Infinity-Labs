#include "square.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Square::Square(const ilrd::Point& center, double width, double angle,
               COLORS color)
    : Shape(center, angle, color), m_width(width)
{
}

Square& Square::Draw()
{
  double half = m_width / 2.0;
  double angle = GetAngle();
  ilrd::Point center = GetCenter();

  ilrd::Point p1(center.GetX() - half, center.GetY() - half);
  ilrd::Point p2(center.GetX() + half, center.GetY() - half);
  ilrd::Point p3(center.GetX() + half, center.GetY() + half);
  ilrd::Point p4(center.GetX() - half, center.GetY() + half);

  p1.Revolve(center, angle);
  p2.Revolve(center, angle);
  p3.Revolve(center, angle);
  p4.Revolve(center, angle);

  DrawPolygon(GetColor(), 4, (int)p1.GetX(), (int)p1.GetY(), (int)p2.GetX(),
              (int)p2.GetY(), (int)p3.GetX(), (int)p3.GetY(), (int)p4.GetX(),
              (int)p4.GetY());

  return *this;
}

Square* Square::Clone() const
{
  return new Square(GetCenter(), m_width, GetAngle(), GetColor());
}