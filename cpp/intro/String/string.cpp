#include "string.hpp"
#include <cctype>
#include <cstring>
#include <iostream>

/************************************************************
Description: Complex
Developer: Steve
Reviewer: Lior
Version: 1
************************************************************/

namespace hrd42
{
//  class functions
String::String(const char* str)
{
  m_str = new char[strlen(str) + 1];
  strcpy(this->m_str, str);
}
String::String(const String& other)
{
  m_str = new char[strlen(other.m_str) + 1];
  strcpy(this->m_str, other.m_str);
}
String& String::operator=(const String& other)
{
  if (this != &other)
  {
    delete[] m_str;
    m_str = new char[strlen(other.m_str) + 1];
    strcpy(m_str, other.m_str);
  }
  return *this;
}
String::~String()
{
  delete[] this->m_str;
}

bool String::operator==(const String& other) const
{
  return (0 == strcmp(this->m_str, other.m_str));
}
bool String::operator!=(const String& other) const
{
  return (!(*this == other));
}
String& String::operator+=(const String& other)
{
  size_t old_len = strlen(this->m_str);
  size_t new_len = old_len + strlen(other.m_str);
  char* new_str = new char[new_len + 1];

  strcpy(new_str, this->m_str);
  strcat(new_str, other.m_str);

  delete[] this->m_str;
  this->m_str = new_str;

  return *this;
}

char& String::operator[](size_t idx)
{
  return this->m_str[idx];
}
const char& String::operator[](size_t idx) const
{
  return this->m_str[idx];
}

size_t String::Length() const
{
  return strlen(this->m_str);
}
const char* String::Cstr() const
{
  return this->m_str;
}

//  global functions
const String operator+(const String& lhs, const String& rhs)
{
  return String(lhs) += rhs;
}
std::istream& operator>>(std::istream& is, String& str)
{
  const size_t chunk_size = 128;
  char buf[chunk_size];
  char* curr = buf;
  char* end = buf + chunk_size - 1;

  str = "";

  while (is.get(*curr) && !std::isspace(*curr))
  {
    ++curr;
    if (curr == end)
    {
      *curr = '\0';
      str += String(buf);
      curr = buf;
    }
  }

  if (curr != buf)
  {
    *curr = '\0';
    str += String(buf);
  }

  return is;
}
std::ostream& operator<<(std::ostream& os, const String& str)
{
  os << str.m_str;

  return os;
}
} // namespace hrd42