#include "rcstring.hpp"

#include <cctype>
#include <cstring>
#include <iostream>

namespace hrd42
{

class RCString::SharedData
{
public:
  char* GetData() const;
  size_t GetCount() const;

  SharedData& operator++();
  SharedData& operator--();

  static SharedData* Create(const char* str);
  static void Destroy(SharedData* data);

private:
  size_t m_count;
  char m_data[];

  explicit SharedData(const char* str, size_t len);
  ~SharedData();

  SharedData(const SharedData&) = delete;
  SharedData& operator=(const SharedData&) = delete;
};

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

char* RCString::SharedData::GetData() const
{
  return const_cast<char*>(m_data);
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

void RCString::Detach()
{
  if (1 == m_data->GetCount())
  {
    return;
  }

  --(*m_data);
  m_data = SharedData::Create(m_data->GetData());
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
  char* tmp = new char[new_len + 1];

  strcpy(tmp, m_data->GetData());
  strcat(tmp, other.m_data->GetData());

  RCString new_str(tmp);
  delete[] tmp;

  std::swap(m_data, new_str.m_data);

  return *this;
}

char& RCString::operator[](size_t idx)
{
  Detach();
  return m_data->GetData()[idx];
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