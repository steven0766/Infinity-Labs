#include "rcstring.hpp"

#include <assert.h>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>

/************************************************************
Description: rcstring
Developer: Steve
Reviewer: rozaline
Version: 1 - ready for review
************************************************************/

namespace hrd42
{

class RCString::SharedData
{
public:
  char* GetData();
  const char* GetData() const;
  size_t GetCount() const;

  SharedData& operator++();
  SharedData& operator--();

  static SharedData* Create(const char* str);
  static void Destroy(SharedData* data);

private:
  size_t m_count;
  char m_data[1];

  explicit SharedData(const char* str, size_t len);
  ~SharedData();

  SharedData(const SharedData&);
  SharedData& operator=(const SharedData&);
};

void RCString::WriteAt(size_t idx, char c)
{
  RCString tmp(Cstr());
  std::swap(m_data, tmp.m_data);
  m_data->GetData()[idx] = c;
}

RCString::ProxyChar::ProxyChar(RCString& rcstr, size_t idx)
    : m_rcstr(rcstr), m_idx(idx)
{
}

RCString::ProxyChar& RCString::ProxyChar::operator=(const char& c)
{
  m_rcstr.WriteAt(m_idx, c);
  return *this;
}

RCString::ProxyChar& RCString::ProxyChar::operator=(const ProxyChar& other)
{

  char c = static_cast<char>(other);
  *this = c;
  return *this;
}

RCString::ProxyChar::operator char() const
{
  return m_rcstr.Cstr()[m_idx];
}

RCString::ProxyChar RCString::operator[](size_t idx)
{
  assert(idx < Length());
  return ProxyChar(*this, idx);
}

RCString::SharedData::SharedData(const char* str, size_t len) : m_count(1)
{
  memcpy(m_data, str, len + 1);
}

RCString::SharedData::~SharedData()
{
}

RCString::SharedData* RCString::SharedData::Create(const char* str)
{
  size_t len = strlen(str);
  void* raw = ::operator new(offsetof(SharedData, m_data) + len + 1);
  return new (raw) SharedData(str, len);
}

void RCString::SharedData::Destroy(SharedData* data)
{
  data->~SharedData();
  ::operator delete(data);
}

RCString::SharedData& RCString::SharedData::operator++()
{
  ++m_count;
  return *this;
}

RCString::SharedData& RCString::SharedData::operator--()
{
  --m_count;
  return *this;
}

char* RCString::SharedData::GetData()
{
  return m_data;
}

const char* RCString::SharedData::GetData() const
{
  return m_data;
}

size_t RCString::SharedData::GetCount() const
{
  return m_count;
}

RCString::RCString(const char* str) : m_data(SharedData::Create(str))
{
}

RCString::RCString(const RCString& other) : m_data(other.m_data)
{
  ++(*m_data);
}

RCString& RCString::operator=(const RCString& other)
{
  RCString tmp(other);
  std::swap(m_data, tmp.m_data);
  return *this;
}

RCString::~RCString()
{
  if (1 == m_data->GetCount())
  {
    SharedData::Destroy(m_data);
  }
  else
  {
    --(*m_data);
  }
}

bool RCString::operator==(const RCString& other) const
{
  return (m_data == other.m_data) ||
         (0 == strcmp(m_data->GetData(), other.m_data->GetData()));
}

bool RCString::operator!=(const RCString& other) const
{
  return !(*this == other);
}

bool RCString::operator>(const RCString& other) const
{
  return other < *this;
}

bool RCString::operator<(const RCString& other) const
{
  return (0 > strcmp(m_data->GetData(), other.m_data->GetData()));
}

RCString& RCString::operator+=(const RCString& other)
{
  size_t new_len = strlen(m_data->GetData()) + strlen(other.m_data->GetData());
  std::vector<char> buf(new_len + 1);

  strcpy(buf.data(), m_data->GetData());
  strcat(buf.data(), other.m_data->GetData());

  RCString tmp(buf.data());
  std::swap(m_data, tmp.m_data);

  return *this;
}

const char& RCString::operator[](size_t idx) const
{
  return m_data->GetData()[idx];
}

size_t RCString::Length() const
{
  return strlen(m_data->GetData());
}

const char* RCString::Cstr() const
{
  return m_data->GetData();
}

const RCString operator+(const RCString& lhs, const RCString& rhs)
{
  return RCString(lhs) += rhs;
}

std::ostream& operator<<(std::ostream& os, const RCString& str)
{
  os << str.m_data->GetData();
  return os;
}

std::istream& operator>>(std::istream& is, RCString& str)
{
  const size_t CHUNK_SIZE = 128;
  char buf[CHUNK_SIZE];
  char* curr = buf;
  char* end = buf + CHUNK_SIZE - 1;

  while (!is.eof() && std::isspace(is.peek()))
  {
    is.ignore();
  }

  str = "";

  while (is.get(*curr) && !std::isspace(*curr))
  {
    ++curr;
    if (end == curr)
    {
      *curr = '\0';
      str += RCString(buf);
      curr = buf;
    }
  }

  if (curr != buf)
  {
    *curr = '\0';
    str += RCString(buf);
  }

  return is;
}

} // namespace hrd42