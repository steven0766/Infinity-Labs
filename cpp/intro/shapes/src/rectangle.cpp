#include "rectangle.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Rectangle::Rectangle(const Point& center, double width, double height,
                     double angle, COLORS color)
    : Shape(center, angle, color), m_width(width), m_height(height)
{
}

Rectangle& Rectangle::Draw()
{
  double half_width = m_width / 2;
  double half_height = m_height / 2;
  double angle = GetAngle();
  Point center = GetCenter();

  Point p1(center.GetX() - half_width, center.GetY() - half_height);
  Point p2(center.GetX() + half_width, center.GetY() - half_height);
  Point p3(center.GetX() + half_width, center.GetY() + half_height);
  Point p4(center.GetX() - half_width, center.GetY() + half_height);

  p1.Revolve(center, angle);
  p2.Revolve(center, angle);
  p3.Revolve(center, angle);
  p4.Revolve(center, angle);

  DrawPolygon(GetColor(), 4, (int)p1.GetX(), (int)p1.GetY(), (int)p2.GetX(),
              (int)p2.GetY(), (int)p3.GetX(), (int)p3.GetY(), (int)p4.GetX(),
              (int)p4.GetY());

  return *this;
}

Rectangle* Rectangle::Clone() const
{
  return new Rectangle(GetCenter(), m_width, m_height, GetAngle(), GetColor());
}