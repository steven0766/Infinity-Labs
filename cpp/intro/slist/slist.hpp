#ifndef ILRD_RD42_SLIST_HPP
#define ILRD_RD42_SLIST_HPP

#include <cstddef> //size_t

namespace hrd42
{

/****************************
Description:
Developer:
Reviewer:
Version:
****************************/
class Slist
{
public:
  explicit Slist();
  ~Slist();

  void PushFront(int val_);
  int PopFront();
  size_t Size() const;
  bool IsEmpty() const;

private:
  class Node;

  Node* m_head;

  Slist(const Slist& other_);
  Slist& operator=(const Slist& other_);
};

} // namespace hrd42

#endif // ILRD_RD42_SLIST_HPP
