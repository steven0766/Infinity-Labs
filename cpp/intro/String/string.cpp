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
  return strcmp(m_str, other.m_str);
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
}