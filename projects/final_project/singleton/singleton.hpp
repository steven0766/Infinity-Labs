#ifndef __ILRD_HRD42_SINGLETON_HPP__
#define __ILRD_HRD42_SINGLETON_HPP__

#include <atomic> //atomic
#include <memory> //unique_ptr
#include <mutex>  //mutex

namespace hrd42
{
template <typename T> class Singleton
{
public:
  template <typename... Args>
  // use only get instance and not s_p_instance directly
  static T* GetInstance(Args... arg);
  Singleton() = delete;
  Singleton(const Singleton&) = delete;
  ~Singleton() = delete;
  Singleton& operator=(const Singleton&) = delete;

private:
  static std::atomic<T*> s_p_instance;
  static std::mutex s_mutex;
};

template <typename T> std::atomic<T*> Singleton<T>::s_p_instance = nullptr;

template <typename T> std::mutex Singleton<T>::s_mutex;

template <typename T>
template <typename... Args>
T* Singleton<T>::GetInstance(Args... arg)
{
  if (nullptr == s_p_instance.load())
  {
    std::lock_guard<std::mutex> lock(s_mutex);
    if (nullptr == s_p_instance.load())
    {
      static std::unique_ptr<T> tmp(new T(arg...));
      s_p_instance.store(tmp.get());
    }
  }

  return s_p_instance.load();
}

} // namespace hrd42

#endif //__ILRD_HRD42_SINGLETON_HPP__