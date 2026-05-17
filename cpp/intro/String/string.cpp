#include "string.hpp"
#include <cstring>

using namespace hrd42;

String::String(const char* str)
{
  strcpy(this->m_str, str);
}
String::String(const String& other)
{
  *this = other;
}
String& String::operator=(const String& other)
{
}
String::~String()
{
}

bool String::operator==(const String& other) const
{
}
bool String::operator!=(const String& other) const
{
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