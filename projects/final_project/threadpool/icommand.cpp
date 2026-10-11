#include "icommand.hpp"

using namespace hrd42;

ICommand::ICommand(Priority priority_) : m_priority(priority_)
{
}

bool ICommand::operator<(const ICommand& comd) const
{
  return m_priority < comd.m_priority;
}