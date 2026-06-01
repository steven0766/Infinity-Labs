#include "point.hpp"
#include <cmath>

using namespace hrd42;

Point::Point(int x_, int y_) : x(x_), y(y_)
{
}

void Point::AdjustBy(Point& other_)
{
  x += other_.x;
  y += other_.y;
}

double Point::Length() const
{
  return sqrt(pow(x, 2) + pow(y, 2));
}

void Point::Print(char prefix_, char suffix_) const
{
  std::cout << prefix_ << x << ", " << y << suffix_ << std::endl;
}

Point Point::Add(Point& p1, Point& p2)
{
}