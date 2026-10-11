#ifndef __ILRD_HRD42_SINGLETON_HPP__
#define __ILRD_HRD42_SINGLETON_HPP__

#include <memory> //unique_ptr
#include <mutex>

namespace hrd42
{
template <typename T> class Singleton
{
public:
  template <typename... Args>
  // use only get instance and not s_p_instance directly
  static T& GetInstance(Args... arg);
  Singleton() = delete;
  Singleton(const Singleton&) = delete;
  ~Singleton() = delete;
  Singleton& operator=(const Singleton&) = delete;

private:
  static std::atomic<T*> s_p_instance;
  static std::unique_ptr<T> m_owner;
  static std::mutex m_mutex;
};

template <typename T>
static std::atomic<T*> Singleton<T>::s_p_instance = nullptr;

template <typename T> static std::unique_ptr<T> m_owner = nullptr;

template <typename T> static std::mutex Singleton<T>::m_mutex;

template <typename T>
template <typename... Args>
T& Singleton<T>::GetInstance(Args... arg)
{
  T* tmp = s_p_instance.load(std::memory_order_acquire);

  if (nullptr == tmp)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    tmp = s_p_instance.load(std::memory_order_relaxed);

    if (nullptr == tmp)
    {
      m_owner = std::make_unique<T>(arg...);
      tmp = m_owner.get();
      s_p_instance.store(tmp, std::memory_order_release);
    }
  }

  return *tmp;
}

} // namespace hrd42

#endif // __ILRD_HRD42_SINGLETON_HPP__