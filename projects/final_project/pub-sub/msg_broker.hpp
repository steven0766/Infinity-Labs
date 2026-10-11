#ifndef MSG_BROKER_ILRD_HRD42_HPP
#define MSG_BROKER_ILRD_HRD42_HPP

#include <exception>  //runtime_error
#include <functional> // std::function
#include <vector>

namespace hrd42
{
template <typename Msg> class ICallback;

template <typename Msg> class Dispatcher // uncopiable
{
public:
  explicit Dispatcher(); //
  Dispatcher(const Dispatcher& other) = delete;
  Dispatcher& operator=(const Dispatcher& other) = delete;
  ~Dispatcher();

  // registering in disconnected state throws runtime_error
  // registering the same callback more than once is undefined
  void Register(ICallback<Msg>* callback);
  void Unregister(ICallback<Msg>* callback);

  void NotifyAll(const Msg& message) const;
  void NotifyDisconnect();

private:
  std::vector<ICallback<Msg>*> m_callbacks;
  bool m_isConnected;
};

template <typename Msg> class ICallback // uncopiable
{
public:
  // may throw runtime_error if dispatcher is disconnected
  explicit ICallback(Dispatcher<Msg>* dispatcher);
  ICallback(const ICallback& other) = delete;
  ICallback& operator=(const ICallback& other) = delete;
  virtual ~ICallback();

  virtual void Notify(const Msg& message) const = 0;
  virtual void NotifyDisconnect() const = 0;

private:
  Dispatcher<Msg>* m_dispatcher;
};

template <typename Msg, typename Subscriber>
class Callback : public ICallback<Msg> // uncopiable
{
public:
  using ActionMethod = void (Subscriber::*)(const Msg& message);
  using DisconnectMethod = void (Subscriber::*)();

  // may throw runtime_error if dispatcher is disconnected
  explicit Callback(Dispatcher<Msg>* dispatcher, Subscriber& subscriber,
                    ActionMethod action, DisconnectMethod disconnect = nullptr);
  Callback(const Callback& other) = delete;
  Callback& operator=(const Callback& other) = delete;
  virtual ~Callback();

  void Notify(const Msg& message) const override;
  void NotifyDisconnect() const override;

private:
  Subscriber& m_subscriber;
  ActionMethod m_actionMethod;
  DisconnectMethod m_disconnectMethod;
};

template <typename Msg> Dispatcher<Msg>::Dispatcher() : m_isConnected(true)
{
}

template <typename Msg> Dispatcher<Msg>::~Dispatcher()
{
  if (m_isConnected)
  {
    NotifyDisconnect();
  }
}

template <typename Msg> void Dispatcher<Msg>::Register(ICallback<Msg>* callback)
{
  if (!m_isConnected)
  {
    throw std::runtime_error("Dispatcher is disconnected");
  }

  m_callbacks.push_back(callback);
}

template <typename Msg>
void Dispatcher<Msg>::Unregister(ICallback<Msg>* callback)
{
  m_callbacks.erase(
      std::remove(m_callbacks.begin(), m_callbacks.end(), callback),
      m_callbacks.end());
}

template <typename Msg>
void Dispatcher<Msg>::NotifyAll(const Msg& message) const
{
  for (auto cb : m_callbacks)
  {
    cb->Notify(message);
  }
}

template <typename Msg> void Dispatcher<Msg>::NotifyDisconnect()
{
  for (auto* i : m_callbacks)
  {
    i->NotifyDisconnect();
  }

  m_isConnected = false;
}

template <typename Msg>
ICallback<Msg>::ICallback(Dispatcher<Msg>* dispatcher)
    : m_dispatcher(dispatcher)
{
  m_dispatcher->Register(this);
}

template <typename Msg> ICallback<Msg>::~ICallback()
{
  m_dispatcher->Unregister(this);
}

//  subscriber
template <typename Msg, typename Subscriber>
Callback<Msg, Subscriber>::Callback(Dispatcher<Msg>* dispatcher,
                                    Subscriber& subscriber, ActionMethod action,
                                    DisconnectMethod disconnect)
    : ICallback<Msg>(dispatcher), m_subscriber(subscriber),
      m_actionMethod(action), m_disconnectMethod(disconnect)
{
}

template <typename Msg, typename Subscriber>
Callback<Msg, Subscriber>::~Callback()
{
}

template <typename Msg, typename Subscriber>
void Callback<Msg, Subscriber>::Notify(const Msg& message) const
{
  (m_subscriber.*m_actionMethod)(message);
}

template <typename Msg, typename Subscriber>
void Callback<Msg, Subscriber>::NotifyDisconnect() const
{
  (m_subscriber.*m_disconnectMethod)();
}

} // namespace hrd42

#endif // MSG_BROKER_ILRD_HRD42_HPP