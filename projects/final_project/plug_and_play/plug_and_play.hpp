#ifndef ILRD_HRD42_PNP_HPP
#define ILRD_HRD42_PNP_HPP

#include <memory> // unique_ptr, make_unique
#include <string>
#include <thread>
#include <vector>

#include "msg_broker.hpp" // Dispatcher, ICommand

namespace hrd42
{

class DirMonitor // uncopiable
{
  class Inotify;

public:
  DirMonitor(const std::string& dir_path);
  DirMonitor(const DirMonitor&) = delete;
  DirMonitor& operator=(const DirMonitor&) = delete;
  ~DirMonitor();

  Dispatcher<std::string>* GetDispatcher() const;

private:
  std::unique_ptr<Dispatcher<std::string>> m_dispatcher;
  std::thread m_monitor;
  std::unique_ptr<Inotify> m_inotify;

  void MonitorThread(const std::string& dir_path);
};

class DirMonitor::Inotify
{
public:
  Inotify(const std::string& dir_path);
  Inotify(const Inotify&) = delete;
  Inotify& operator=(const Inotify&) = delete;
  ~Inotify();

  void WaitForEvent(Dispatcher<std::string>* dispatcher_);

private:
  int m_Fd;
  int m_wd;
};

class SoLoader // uncopiable
{
public:
  SoLoader(Dispatcher<std::string>* dispatcher);
  SoLoader(const SoLoader& other) = delete;
  SoLoader& operator=(const SoLoader& other) = delete;
  ~SoLoader();

private:
  std::unique_ptr<Callback<std::string, SoLoader>> m_callback;
  std::vector<void*> m_soHandles;

  void OnLoad(const std::string& dir_path);
  void OnDisconnect();
};

class PnP
{
public:
  PnP(const std::string& dir_path);
  PnP(const PnP& other) = delete;
  PnP& operator=(const PnP& other) = delete;
  ~PnP() = default;

private:
  DirMonitor m_monitor;
  SoLoader m_loader;
};

} // namespace hrd42

#endif // ILRD_HRD42_PNP_HPP