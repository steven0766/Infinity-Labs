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
template <typename T> class Slist
{
public:
  explicit Slist();
  ~Slist();

  void PushFront(T val_);
  T PopFront();
  size_t Size() const;
  bool IsEmpty() const;

private:
  class Node
  {
  public:
    explicit Node(T val_, Node* const next_ = 0);
    ~Node();
    Node* GetNext() const;
    T GetVal() const;

  private:
    Node(const Node& other);
    Node& operator=(const Node& other);
    T m_val;
    Node* m_next;
  };

  Node* m_head;

  Slist(const Slist& other_);
  Slist& operator=(const Slist& other_);
};

// Slist functions
template <typename T> Slist<T>::Slist() : m_head(0)
{
}
template <typename T> Slist<T>::~Slist()
{
  Slist<T>::Node* curr = this->m_head;
  Slist<T>::Node* next = this->m_head;

  while (curr)
  {
    next = curr->GetNext();
    delete curr;
    curr = 0;
    curr = next;
  }
}
template <typename T> void Slist<T>::PushFront(T val_)
{
  Slist<T>::Node* new_node = new Node(val_, this->m_head);
  this->m_head = new_node;
}
template <typename T> T Slist<T>::PopFront()
{
  T data = this->m_head->GetVal();
  Slist<T>::Node* remove_node = this->m_head;
  this->m_head = this->m_head->GetNext();
  delete remove_node;
  return data;
}
template <typename T> size_t Slist<T>::Size() const
{
  Slist<T>::Node* curr = this->m_head;
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
template <typename T> Slist<T>::Node::~Node()
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
