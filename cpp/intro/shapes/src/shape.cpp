#include "shape.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Shape::Shape(const ilrd::Point& center, double angle = 0,
             COLORS color = COLOR_WHITE)
    : m_center(center), m_angle(angle), m_color(color)
{
}
Shape::~Shape()
{
}

Shape& Shape::Draw() const
{
}
Shape& Shape::Move(const ilrd::Point& dest)
{
}
Shape& Shape::Move(double x_offest, double y_offset)
{
}
Shape& Shape::Rotate(double angle)
{
}
Shape& Shape::Revolve(const ilrd::Point& pivot, double angle)
{
}
Shape& Shape::SetColor(COLORS color)
{
}

Shape* Shape::Clone() const
{
}