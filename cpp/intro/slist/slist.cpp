#include "slist.hpp"
using namespace hrd42;
// cpp:

/****************************
Description:
Developer:
Reviewer:
Version:
****************************/
class Slist::Node
{
public:
  explicit Node(int val_, Node* const next_ = 0);
  ~Node();
  Node* GetNext() const;
  int GetVal() const;

private:
  Node(const Node& other);
  Node& operator=(const Node& other);
  int m_val;
  Node* m_next;
}; // namespace Slist::Node

Slist::Node::Node(int val_, Node* const next_) : m_val(val_), m_next(next_)
{
}
Slist::Node::~Node()
{
}
Slist::Node* Slist::Node::GetNext() const
{
  return m_next;
}

int Slist::Node::GetVal() const
{
  return m_val;
}

Slist::Slist() : m_head(0)
{
}
Slist::~Slist()
{
  Node* curr = this->m_head;
  Node* next = curr;

  while (curr)
  {
    next = next->GetNext();
    delete curr;
    curr = 0;
    curr = next;
  }

  this->m_head = 0;
}
void Slist::PushFront(int val_)
{
  Node* new_node = new Node(val_, this->m_head);
  this->m_head = new_node;
}
int Slist::PopFront()
{
  Node* node = this->m_head;
  int ret = node->GetVal();
  this->m_head = node->GetNext();
  delete node;
  return ret;
}
size_t Slist::Size() const
{
  size_t ret = 0;
  Node* curr = this->m_head;

  while (curr)
  {
    ++ret;
    curr = curr->GetNext();
  }

  return ret;
}
bool Slist::IsEmpty() const
{
  return 0 == this->m_head;
}