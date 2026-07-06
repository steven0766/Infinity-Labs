#include "plug_and_play.hpp"
#include "msg_broker.hpp"

using namespace hrd42;

PnP::PnP(const std::string& dir_path)
    : m_monitor(dir_path), m_loader(m_monitor.GetDispatcher())
{
}

// DirMonitor

DirMonitor::DirMonitor(const std::string& dir_path)
    : m_dispatcher(std::make_unique<Dispatcher<std::string>>()),
      m_monitor(&DirMonitor::MonitorThread, this, dir_path)
{
}

void DirMonitor::MonitorThread(const std::string& dir_path)
{
}