template <typename T> class Square
{
public:
  Square(T len = 2);
  T GetLen() const;
  void SetLen(T len);

private:
  T m_length;
};

template <typename T> Square<T>::Square(T len_) : m_length(len_)
{
}
template <typename T> T Square<T>::GetLen() const
{
  return m_length;
}
template <typename T> void Square<T>::SetLen(T len_)
{
  m_length = len_;
}