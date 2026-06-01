#ifndef __ILRD_HRD42__POINT__
#define __ILRD_HRD42__POINT__

#include <iostream>

namespace hrd42
{
struct Point
{
public:
  Point(int x, int y);
  void AdjustBy(Point& other);
  double Length() const;
  void Print(char prefix, char suffix) const;
  Point Add(Point& p1, Point& p2);

private:
  int x;
  int y;
};
}; // namespace hrd42

#endif // __ILRD_HRD42__POINT__