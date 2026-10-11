#ifndef ILRD_HRD42_FACTORY_HPP
#define ILRD_HRD42_FACTORY_HPP

#include <functional>    // function
#include <memory>        // shared_ptr
#include <unordered_map> // unordered_map

#include "singleton.hpp" // Singleton

namespace hrd42
{
template <typename Base, typename Key, typename... Args>
class Factory // non-copyable
{
public:
  using CreateFunc = std::function<std::shared_ptr<Base>(Args... args)>;

  void Add(const Key& key,
           CreateFunc create); // "create" for "key" can be overwritten by
                               // calling this function
  std::shared_ptr<Base>
  Create(const Key& key,
         Args... args); // may throw std::out_of_range if the key doesn't exist
  ~Factory() = default;

private:
  std::unordered_map<Key, CreateFunc> m_map;
  Factory(const Factory&) = delete;
  Factory& operator=(const Factory&) = delete;
  Factory() = default;

  friend class Singleton<Factory>;
};

template <typename Base, typename Key, typename... Args>
void Factory<Base, Key, Args...>::Add(const Key& key, CreateFunc func)
{
  m_map[key] = func;
}

template <typename Base, typename Key, typename... Args>
std::shared_ptr<Base> Factory<Base, Key, Args...>::Create(const Key& key,
                                                          Args... args)
{
  return m_map.at(key)(args...);
}

} // namespace hrd42
#endif // ILRD_HRD42_FACTORY_HPP