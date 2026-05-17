#include "string.hpp"
#include <cstring>

using namespace hrd42;

String::String(const char* str)
{
  m_str = new char[strlen(str) + 1];
  strcpy(m_str, str);
}
String::String(const String& other)
{
  m_str = new char[strlen(other.m_str) + 1];
  strcpy(m_str, other.m_str);
}
String& String::operator=(const String& other)
{
  delete[] m_str;
  m_str = new char[strlen(other.m_str) + 1];
  strcpy(m_str, other.m_str);
  return *this;
}
String::~String()
{
  delete[] m_str;
}

bool String::operator==(const String& other) const
{
  return (this->m_str == other.m_str);
}
bool String::operator!=(const String& other) const
{
  return (!(*this == other));
}
String& String::operator+=(const String& other)
{
}

char& String::operator[](size_t idx)
{
}
const char& String::operator[](size_t idx) const
{
}

size_t String::Length() const
{
}
const char* String::Cstr() const
{
}