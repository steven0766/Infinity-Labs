#ifndef ILRD_RD42_SLIST_HPP
#define ILRD_RD42_SLIST_HPP

#include <cstddef> //size_t

/****************************
Description:Slist generic
Developer:Steve
Reviewer:Tomer
Version:1.1
****************************/

namespace hrd42
{
template <typename T> class Slist
{
public:
  explicit Slist();
  ~Slist();

  void PushFront(T val);
  T PopFront();
  size_t Size() const;
  bool IsEmpty() const;

private:
  class Node;

  Node* m_head;

  Slist(const Slist& other);
  Slist& operator=(const Slist& other);
};

template <typename T> class Slist<T>::Node
{
public:
  explicit Node(T val, Node* const next = 0);
  Node* GetNext() const;
  T GetVal() const;

private:
  T m_val;
  Node* m_next;
};

// Slist functions
template <typename T> Slist<T>::Slist() : m_head(0)
{
}
template <typename T> Slist<T>::~Slist()
{
  Node* curr = this->m_head;
  Node* next = this->m_head;

  while (curr)
  {
    next = curr->GetNext();
    delete curr;
    curr = next;
  }
}
template <typename T> void Slist<T>::PushFront(T val_)
{
  Node* new_node = new Node(val_, this->m_head);
  this->m_head = new_node;
}
template <typename T> T Slist<T>::PopFront()
{
  T data = this->m_head->GetVal();
  Node* remove_node = this->m_head;
  this->m_head = this->m_head->GetNext();
  delete remove_node;
  return data;
}
template <typename T> size_t Slist<T>::Size() const
{
  Node* curr = this->m_head;
  size_t cnt = 0;

  while (curr)
  {
    ++cnt;
    curr = curr->GetNext();
  }

  return cnt;
}
template <typename T> bool Slist<T>::IsEmpty() const
{
  return (0 == this->m_head);
}

// node's functions
template <typename T>
Slist<T>::Node::Node(T val_, Node* const next_) : m_val(val_), m_next(next_)
{
}
template <typename T> typename Slist<T>::Node* Slist<T>::Node::GetNext() const
{
  return this->m_next;
}
template <typename T> T Slist<T>::Node::GetVal() const
{
  return this->m_val;
}

} // namespace hrd42

#endif // ILRD_RD42_SLIST_HPP
