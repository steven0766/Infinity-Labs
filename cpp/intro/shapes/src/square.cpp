#include "square.hpp"
#include "glut_utils.h"
#include "point.hpp"

using namespace hrd42;
using namespace ilrd;

Square::Square(const ilrd::Point& center, double width = 1, double angle = 0,
               COLORS color = COLOR_WHITE)
    : Shape(center, angle, color), m_width(width)
{
}