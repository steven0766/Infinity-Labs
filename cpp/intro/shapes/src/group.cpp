#include "group.hpp"
using namespace hrd42;
using namespace ilrd;

Group::Group(Shape& shape) : Shape(Point(0, 0)), m_shapes()
{
  m_shapes.push_back(&shape);
}

Group& Group::AddShape(Shape* shape)
{
  m_shapes.push_back(shape);
  return *this;
}

Group& Group::Draw()
{
  for (size_t i = 0; i < m_shapes.size(); ++i)
  {
    m_shapes[i]->Draw();
  }
  return *this;
}

Group& Group::Move(const Point& dest)
{
  for (size_t i = 0; i < m_shapes.size(); ++i)
  {
    m_shapes[i]->Move(dest);
  }
  return *this;
}

Group& Group::Move(double x_offset, double y_offset)
{
  for (size_t i = 0; i < m_shapes.size(); ++i)
  {
    m_shapes[i]->Move(x_offset, y_offset);
  }
  return *this;
}

Group& Group::Rotate(double angle)
{
  for (size_t i = 0; i < m_shapes.size(); ++i)
  {
    m_shapes[i]->Rotate(angle);
  }
  return *this;
}

Group& Group::Revolve(const Point& pivot, double angle)
{
  for (size_t i = 0; i < m_shapes.size(); ++i)
  {
    m_shapes[i]->Revolve(pivot, angle);
  }
  return *this;
}

Group& Group::SetColor(COLORS color)
{
  for (size_t i = 0; i < m_shapes.size(); ++i)
  {
    m_shapes[i]->SetColor(color);
  }
  return *this;
}

Group* Group::Clone() const
{
  Group* new_group = new Group(*m_shapes[0]->Clone());
  for (size_t i = 1; i < m_shapes.size(); ++i)
  {
    new_group->AddShape(m_shapes[i]->Clone());
  }
  return new_group;
}