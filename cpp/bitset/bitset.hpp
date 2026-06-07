/************************************************************
Description: BitSet class
Developer:
Reviewer:
Version: 1
************************************************************/
#ifndef ILRD_HRD42_BITSET_HPP
#define ILRD_HRD42_BITSET_HPP

#include <cstddef>  // size_t
#include <iostream> // istream, ostream
#include <numeric>  //accumulate
#include <vector>   // vector

namespace hrd42
{
template <size_t Size> class BitSet;

template <size_t Size>
std::ostream& operator<<(std::ostream& os, const BitSet<Size>& bitset);

// class declaration
template <size_t Size> class BitSet
{
private:
  class BitProxy;

public:
  explicit BitSet();
  // using generated cctor, copy= & dtor

  bool operator!=(const BitSet& other) const;
  bool operator==(const BitSet& other) const;

  BitSet& operator&=(const BitSet& other);
  BitSet& operator|=(const BitSet& other);
  BitSet& operator^=(const BitSet& other);

  BitProxy operator[](size_t idx);
  bool operator[](size_t idx) const;

  size_t CountTrueBits() const;

private:
  std::vector<size_t> m_bitset;
  static size_t SumTrueBits(size_t cnt, size_t block);
  void SetBit(size_t idx, bool val);
  bool GetBit(size_t idx) const;

  friend std::ostream& operator<< <Size>(std::ostream& os,
                                         const BitSet& bitset);
};

template <size_t Size> class BitSet<Size>::BitProxy
{
public:
  explicit BitProxy(BitSet<Size>& bitset, size_t idx);
  BitProxy& operator=(const BitProxy& other); // b[i1] = b[i2]
  // using generater cctor & dtor

  BitProxy& operator=(bool val); // b[i] = bool
  operator bool() const;

private:
  BitSet<Size>& m_bitset;
  size_t m_idx;
};

template <size_t Size>
BitSet<Size>::BitSet()
    : m_bitset((Size + sizeof(size_t) * 8 - 1) / (sizeof(size_t) * 8), 0)
{
}
template <size_t Size> bool BitSet<Size>::operator!=(const BitSet& other) const
{
  return m_bitset != other.m_bitset;
}

template <size_t Size> bool BitSet<Size>::operator==(const BitSet& other) const
{
  return m_bitset == other.m_bitset;
}

template <size_t Size>
BitSet<Size>& BitSet<Size>::operator&=(const BitSet& other)
{
  for (size_t i = 0; i < this->m_bitset.size(); ++i)
  {
    this->m_bitset[i] &= other.m_bitset[i];
  }

  return *this;
}

template <size_t Size>
BitSet<Size>& BitSet<Size>::operator|=(const BitSet& other)
{
  for (size_t i = 0; i < this->m_bitset.size(); ++i)
  {
    this->m_bitset[i] |= other.m_bitset[i];
  }

  return *this;
}
template <size_t Size>
BitSet<Size>& BitSet<Size>::operator^=(const BitSet& other)
{
  for (size_t i = 0; i < this->m_bitset.size(); ++i)
  {
    this->m_bitset[i] ^= other.m_bitset[i];
  }

  return *this;
}

template <size_t Size>
typename BitSet<Size>::BitProxy BitSet<Size>::operator[](size_t idx)
{
  return BitProxy(*this, idx);
}

template <size_t Size> bool BitSet<Size>::operator[](size_t idx) const
{
  return GetBit(idx);
}

template <size_t Size> size_t BitSet<Size>::CountTrueBits() const
{
  return accumulate(m_bitset.begin(), m_bitset.end(), static_cast<size_t>(0),
                    SumTrueBits);
}

template <size_t Size>
size_t BitSet<Size>::SumTrueBits(size_t sum, size_t block)
{
  size_t cnt = 0;
  while (block)
  {
    cnt += block & 1;
    block >>= 1;
  }

  return sum + cnt;
}

template <size_t Size> void BitSet<Size>::SetBit(size_t idx, bool val)
{
  size_t bit_idx = idx % (sizeof(size_t) * 8);
  size_t vec_idx = idx / (sizeof(size_t) * 8);
  if (val)
  {
    m_bitset.at(vec_idx) |= (static_cast<size_t>(1) << bit_idx);
  }
  else
  {
    m_bitset.at(vec_idx) &= (~(static_cast<size_t>(1) << bit_idx));
  }
}

template <size_t Size> bool BitSet<Size>::GetBit(size_t idx) const
{
  size_t bit_idx = idx % (sizeof(size_t) * 8);
  size_t vec_idx = idx / (sizeof(size_t) * 8);
  return ((m_bitset.at(vec_idx) >> bit_idx) & static_cast<size_t>(1));
}

template <size_t Size>
std::ostream& operator<<(std::ostream& os, const BitSet<Size>& bitset)
{
  for (size_t i = Size; i > 0; --i)
  {
    os << bitset.GetBit(i - 1);
  }

  return os;
}

template <size_t Size>
BitSet<Size>::BitProxy::BitProxy(BitSet<Size>& bitset, size_t idx)
    : m_bitset(bitset), m_idx(idx)
{
}

template <size_t Size>
typename BitSet<Size>::BitProxy&
BitSet<Size>::BitProxy::operator=(const BitProxy& other)
{
  *this = static_cast<bool>(other);
  return *this;
}

template <size_t Size>
typename BitSet<Size>::BitProxy& BitSet<Size>::BitProxy::operator=(bool val)
{
  this->m_bitset.SetBit(m_idx, val);
  return *this;
}

template <size_t Size> BitSet<Size>::BitProxy::operator bool() const
{
  return m_bitset.GetBit(m_idx);
}

} // namespace hrd42

#endif // ILRD_HRD42_BITSET_HPP
