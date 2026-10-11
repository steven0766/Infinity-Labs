#include "threadpool.hpp"
#include <iostream>
#include <pthread.h>

using namespace hrd42;

class thread_termination_exception : public std::exception
{
public:
  const char* what() const noexcept override
  {
    return "Thread safely terminated";
  }
};

//  stop command
class ThreadPool::StopCommand : public ICommand
{
public:
  StopCommand() : ICommand(ADMIN)
  {
  }
  void Execute() override
  {
    throw thread_termination_exception();
  }
};

class ThreadPool::SuspendCommand : public ICommand
{
public:
  SuspendCommand(ThreadPool* pool) : ICommand(ADMIN), m_pool(pool)
  {
  }
  void Execute() override
  {
    std::unique_lock<std::mutex> lock(m_pool->m_suspend_mutex);
    m_pool->m_suspend_cv.wait(lock, [this]
                              { return !m_pool->m_is_suspended.load(); });
  }

private:
  ThreadPool* m_pool;
};

ThreadPool::ThreadPool(size_t size)
    : m_wpq(), m_running(), m_active_threads(size)
{
  CreateThreads(size);
}

void ThreadPool::WorkerFunc()
{
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
  bool is_running = true;

  while (is_running)
  {
    try
    {
      auto cmd = m_wpq.Pop();
      cmd->Execute();
    }
    catch (const thread_termination_exception& e)
    {
      is_running = false;
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      throw;
    }
  }
}

ThreadPool::~ThreadPool()
{
  Stop();
}

void ThreadPool::AddCommand(std::shared_ptr<ICommand> cmd)
{
  m_wpq.Push(cmd);
}

// throw exception instead
void ThreadPool::Stop()
{
  m_is_suspended = false;
  m_suspend_cv.notify_all();
  PushStopCmd(m_running.size());
  JoinAll();
}

void ThreadPool::ForceStop(std::chrono::milliseconds ms_waittime)
{
  Resume();
  PushStopCmd(m_running.size());
  std::this_thread::sleep_for(ms_waittime);
  for (auto& th : m_running)
  {
    if (th.joinable())
    {
      pthread_cancel(th.native_handle());
      th.join();
    }
  }
  m_running.clear();
}

void ThreadPool::SetSize(size_t new_size)
{
  Stop();
  m_running.clear();
  m_active_threads = new_size;
  CreateThreads(new_size);
}

void ThreadPool::Suspend()
{
  m_is_suspended = true;
  for (size_t i = 0; i < m_running.size(); ++i)
  {
    m_wpq.Push(std::make_shared<SuspendCommand>(this));
  }
}

void ThreadPool::Resume()
{
  m_is_suspended = false;
  m_suspend_cv.notify_all();
}

void ThreadPool::CreateThreads(size_t cnt)
{
  for (size_t i = 0; i < cnt; ++i)
  {
    m_running.emplace_back(&ThreadPool::WorkerFunc, this);
  }
}

void ThreadPool::JoinAll()
{
  for (auto& th : m_running)
  {
    if (th.joinable())
    {
      th.join();
    }
  }
}

void ThreadPool::PushStopCmd(size_t count)
{
  for (size_t i = 0; i < count; ++i)
  {
    m_wpq.Push(std::make_shared<StopCommand>());
  }
}