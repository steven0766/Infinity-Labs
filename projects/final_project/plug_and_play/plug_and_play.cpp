#include "plug_and_play.hpp"
#include "msg_broker.hpp"

#include <errno.h>       // errno
#include <limits.h>      //NAME_MAX
#include <sys/inotify.h> // inotify
#include <system_error>  // system_error
#include <unistd.h>      // close, read
#include <vector>        // vector

using namespace hrd42;

PnP::PnP(const std::string& dir_path)
    : m_monitor(dir_path), m_loader(m_monitor.GetDispatcher())
{
}

// DirMonitor

DirMonitor::DirMonitor(const std::string& dir_path)
    : m_dispatcher(std::make_unique<Dispatcher<std::string>>()),
      m_inotify(std::make_unique<Inotify>(dir_path)),
      m_monitor(&DirMonitor::MonitorThread, this, dir_path)
{
}

DirMonitor::~DirMonitor()
{
  pthread_cancel(m_monitor.native_handle());
  m_monitor.join();
}

Dispatcher<std::string>* DirMonitor::GetDispatcher() const
{
  return m_dispatcher.get();
}

void DirMonitor::MonitorThread(const std::string& dir_path)
{
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
  m_inotify->WaitForEvent(m_dispatcher.get());
}

DirMonitor::Inotify::Inotify(const std::string& dir_path)
    : m_fd(inotify_init()), m_wfd(-1)
{
  if (-1 == m_fd)
  {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "inotify_init error");
  }
  m_wfd =
      inotify_add_watch(m_fd, dir_path.c_str(), IN_CLOSE_WRITE | IN_MOVED_TO);

  if (m_wfd == -1)
  {
    close(m_fd);
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "inotify_add_watch failed");
  }
}

DirMonitor::Inotify::~Inotify()
{
  close(m_fd);
}

void DirMonitor::Inotify::WaitForEvent(Dispatcher<std::string>* dispatcher_)
{
  /*
    // while true
    while (true)
    {
      // wait for new so file

      // dispatcher notify with the name of new so file
    }*/
}

void DirMonitor::Inotify::WaitForEvent(Dispatcher<std::string>* dispatcher_)
{
  // allocate buffer big enough for multiple inotify events
  size_t bufSize = sizeof(struct inotify_event) + NAME_MAX + 1;
  std::vector<char> buf(bufSize);

  // while true
  while (true)
  {
    // block directory changes
    ssize_t len = read(m_fd, buf.data(), bufSize);

    if (-1 == len)
    {
      // interrupted by a signal
      if (EINTR == errno)
      {
        // retry the read
        continue;
      }
      // real error - can't recover
      throw std::system_error(std::error_code(errno, std::system_category()),
                              "reading inotify object failed");
    }

    // iterate over all events
    size_t i = 0;
    while (i < static_cast<size_t>(len))
    {
      auto* event = reinterpret_cast<struct inotify_event*>(&buf[i]);

      // if event has valid filename,
      // and only if the file is fully written

      if (event->len > 0 && (event->mask & (IN_CLOSE_WRITE | IN_MOVED_TO)))
      {
        std::string filename(event->name);

        // filter only .so files
        if (filename.length() >= 3 &&
            0 == filename.compare(filename.length() - 3, 3, ".so"))
        {
          // dispatcher notify new so file
          dispatcher_->NotifyAll(filename);
        }
      }
      // go to the next so file
      i += sizeof(struct inotify_event) + event->len;
    }
  }
}