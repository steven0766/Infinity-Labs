/************************************************************
Description: setbit
Developer: Steve
Reviewer: Tomer
Version: 1 - ready for review
         1.1 - after review
************************************************************/
#ifndef ILRD_HRD42_BITSET_HPP
#define ILRD_HRD42_BITSET_HPP

#include <algorithm>  // transform
#include <climits>    // CHAR_BIT
#include <cstddef>    // size_t
#include <functional> // bit_and, bit_or, bit_xor
#include <iostream>   // istream, ostream
#include <numeric>    //accumulate
#include <vector>     // vector

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
    : m_bitset((Size + sizeof(size_t) * CHAR_BIT - 1) /
                   (sizeof(size_t) * CHAR_BIT),
               0)
{
}
template <size_t Size> bool BitSet<Size>::operator!=(const BitSet& other_) const
{
  return m_bitset != other_.m_bitset;
}

template <size_t Size> bool BitSet<Size>::operator==(const BitSet& other_) const
{
  return m_bitset == other_.m_bitset;
}

template <size_t Size>
BitSet<Size>& BitSet<Size>::operator&=(const BitSet& other_)
{
  std::transform(m_bitset.begin(), m_bitset.end(), other_.m_bitset.begin(),
                 m_bitset.begin(), std::bit_and<size_t>());

  return *this;
}

template <size_t Size>
BitSet<Size>& BitSet<Size>::operator|=(const BitSet& other_)
{
  std::transform(m_bitset.begin(), m_bitset.end(), other_.m_bitset.begin(),
                 m_bitset.begin(), std::bit_or<size_t>());

  return *this;
}
template <size_t Size>
BitSet<Size>& BitSet<Size>::operator^=(const BitSet& other_)
{
  std::transform(m_bitset.begin(), m_bitset.end(), other_.m_bitset.begin(),
                 m_bitset.begin(), std::bit_xor<size_t>());

  return *this;
}

template <size_t Size>
typename BitSet<Size>::BitProxy BitSet<Size>::operator[](size_t idx_)
{
  return BitProxy(*this, idx_);
}

template <size_t Size> bool BitSet<Size>::operator[](size_t idx_) const
{
  return GetBit(idx_);
}

template <size_t Size> size_t BitSet<Size>::CountTrueBits() const
{
  return accumulate(m_bitset.begin(), m_bitset.end(), static_cast<size_t>(0),
                    SumTrueBits);
}

template <size_t Size>
size_t BitSet<Size>::SumTrueBits(size_t sum_, size_t block_)
{
  size_t cnt = 0;
  while (block_)
  {
    cnt += block_ & 1;
    block_ >>= 1;
  }

  return sum_ + cnt;
}

template <size_t Size> void BitSet<Size>::SetBit(size_t idx_, bool val)
{
  size_t bit_idx = idx_ % (sizeof(size_t) * CHAR_BIT);
  size_t vec_idx = idx_ / (sizeof(size_t) * CHAR_BIT);
  size_t mask = static_cast<size_t>(1) << bit_idx;
  m_bitset[vec_idx] =
      (m_bitset[vec_idx] & ~mask) | (static_cast<size_t>(val) << bit_idx);
}

template <size_t Size> bool BitSet<Size>::GetBit(size_t idx_) const
{
  size_t bit_idx = idx_ % (sizeof(size_t) * CHAR_BIT);
  size_t vec_idx = idx_ / (sizeof(size_t) * CHAR_BIT);
  return (m_bitset.at(vec_idx) >> bit_idx) & 1;
}

template <size_t Size>
std::ostream& operator<<(std::ostream& os_, const BitSet<Size>& bitset_)
{
  for (size_t i = Size; i > 0; --i)
  {
    os_ << bitset_.GetBit(i - 1);
  }

  return os_;
}

template <size_t Size>
BitSet<Size>::BitProxy::BitProxy(BitSet<Size>& bitset_, size_t idx_)
    : m_bitset(bitset_), m_idx(idx_)
{
}

template <size_t Size>
typename BitSet<Size>::BitProxy&
BitSet<Size>::BitProxy::operator=(const BitProxy& other_)
{
  *this = static_cast<bool>(other_);
  return *this;
}

template <size_t Size>
typename BitSet<Size>::BitProxy& BitSet<Size>::BitProxy::operator=(bool val_)
{
  this->m_bitset.SetBit(m_idx, val_);
  return *this;
}

template <size_t Size> BitSet<Size>::BitProxy::operator bool() const
{
  return m_bitset.GetBit(m_idx);
}

} // namespace hrd42

#endif // ILRD_HRD42_BITSET_HPP
