#ifndef ILRD_HRD42_THREAD_POOL_HPP
#define ILRD_HRD42_THREAD_POOL_HPP

#include <atomic>
#include <chrono>     // std::chrono::milliseconds
#include <cstddef>    // size_t
#include <functional> // std::less
#include <memory>     // shared_ptr
#include <stdexcept>
#include <thread>
#include <vector>

#include "icommand.hpp"
#include "wpq.hpp"

namespace hrd42
{

class ThreadPool // non-copyable
{
public:
  explicit ThreadPool(size_t size = ThreadCnt());
  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool& operator=(const ThreadPool& other) = delete;
  ~ThreadPool();

  void AddCommand(std::shared_ptr<ICommand> cmd);
  void Stop();
  void ForceStop(std::chrono::milliseconds ms_waittime);

  void SetSize(size_t new_size);
  void Suspend();
  void Resume();

private:
  class StopCommand;
  class SuspendCommand;

  struct ICommandCompare
  {
    bool operator()(const std::shared_ptr<ICommand>& a,
                    const std::shared_ptr<ICommand>& b) const
    {
      return *a < *b;
    }
  };

  WPriorityQueue<std::shared_ptr<ICommand>,
                 std::vector<std::shared_ptr<ICommand>>, ICommandCompare>
      m_wpq;
  std::vector<std::thread> m_running;
  size_t m_active_threads;

  std::mutex m_suspend_mutex;
  std::condition_variable m_suspend_cv;
  std::atomic<bool> m_is_suspended;

  static size_t ThreadCnt();
  void WorkerFunc();
  void CreateThreads(size_t count);
  void JoinAll();
  void PushStopCmd(size_t count);
};

inline size_t ThreadPool::ThreadCnt()
{
  return std::max(1u, std::thread::hardware_concurrency());
}
} // namespace hrd42

#endif // ILRD_HRD42_THREAD_POOL_HPP