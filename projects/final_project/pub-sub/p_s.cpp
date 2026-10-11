#ifndef MSG_BROKER_ILRD_HRD42_HPP
#define MSG_BROKER_ILRD_HRD42_HPP
#include <algorithm>  // std::remove   [ADDED - needed for Unregister]
#include <functional> // std::function
#include <stdexcept>  // std::runtime_error   [CHANGED from <exception>]
#include <vector>
namespace hrd42
{
template <typename Msg> class ICallback;

template <typename Msg> class Dispatcher // uncopiable
{
public:
  explicit Dispatcher();
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

  // [CHANGED] no longer pure virtual, no longer const --
  // base implementation flips m_isConnected; derived classes
  // that override MUST call ICallback<Msg>::NotifyDisconnect()
  virtual void NotifyDisconnect();

private:
  Dispatcher<Msg>* m_dispatcher;
  bool m_isConnected =
      true; // [ADDED] tracks whether m_dispatcher is still alive
};

template <typename Msg, typename Subscriber>
class Callback : public ICallback<Msg> // uncopiable
{
public:
  // [CHANGED] std::function instead of pointer-to-member --
  // lets Subscriber use ANY callable with any method name,
  // not just a hardcoded "Update"/"Notify" method
  using ActionMethod = std::function<void(Subscriber*, const Msg& message)>;
  using DisconnectMethod = std::function<void(Subscriber*)>;

  // may throw runtime_error if dispatcher is disconnected
  explicit Callback(Dispatcher<Msg>* dispatcher, Subscriber& subscriber,
                    ActionMethod action, DisconnectMethod disconnect = nullptr);
  Callback(const Callback& other) = delete;
  Callback& operator=(const Callback& other) = delete;
  virtual ~Callback();

  void Notify(const Msg& message) const override;
  void NotifyDisconnect()
      override; // [CHANGED] not const -- calls base which mutates m_isConnected

private:
  Subscriber& m_subscriber;
  ActionMethod m_actionMethod;
  DisconnectMethod m_disconnectMethod;
};

template <typename Msg>
Dispatcher<Msg>::Dispatcher()
    : m_isConnected(true) // [FIXED] was m_is(true) -- typo
{
}

template <typename Msg> Dispatcher<Msg>::~Dispatcher()
{
  this->NotifyDisconnect();
}

template <typename Msg> void Dispatcher<Msg>::Register(ICallback<Msg>* callback)
{
  // [NOTE] per the header comment, this should throw runtime_error
  // if !m_isConnected -- not yet implemented here:
  // if (!m_isConnected) { throw std::runtime_error("Dispatcher is
  // disconnected"); }
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
  for (auto* cb : m_callbacks) // [FIXED] was "auto i" (missing *) -- i->Notify
                               // already correct below
  {
    cb->Notify(message);
  }
}

template <typename Msg> void Dispatcher<Msg>::NotifyDisconnect()
{
  for (ICallback<Msg>* cb :
       m_callbacks) // [FIXED] was "for (ICallback<Msg>* i : Dispatcher)"
  {
    cb->NotifyDisconnect(); // [FIXED] was i.NotifyOOS(message) -- wrong name,
                            // wrong syntax, wrong arg
  }
  m_isConnected = false; // runs once, after the loop -- see explanation above
}

template <typename Msg>
ICallback<Msg>::ICallback(Dispatcher<Msg>* dispatcher)
    : m_dispatcher(dispatcher)
{
  m_dispatcher->Register(this);
}

template <typename Msg> ICallback<Msg>::~ICallback()
{
  if (m_isConnected) // [ADDED] guard against dangling m_dispatcher
  {
    m_dispatcher->Unregister(this);
  }
}

template <typename Msg>
void ICallback<Msg>::NotifyDisconnect() // [ADDED] base implementation
{
  m_isConnected = false;
}

//  subscriber
template <typename Msg, typename Subscriber>
Callback<Msg, Subscriber>::Callback(Dispatcher<Msg>* dispatcher,
                                    Subscriber& subscriber, ActionMethod action,
                                    DisconnectMethod disconnect)
    : ICallback<Msg>(dispatcher), // [FIXED] was ICallback(dispatcher_) -- no
                                  // such param, missing <Msg>
      m_subscriber(subscriber),   // [FIXED] was subscriber_
      m_actionMethod(action), m_disconnectMethod(disconnect)
{
}

template <typename Msg, typename Subscriber>
Callback<Msg, Subscriber>::~Callback()
{
  // [REMOVED] this->NotifyDisconnect();
  // Calling this here fires on EVERY destruction, even when the
  // dispatcher is alive and well, and double-fires if the dispatcher
  // already called NotifyDisconnect on this object first.
  // NotifyDisconnect should only ever be triggered by the Dispatcher.
}

template <typename Msg, typename Subscriber>
void Callback<Msg, Subscriber>::Notify(const Msg& message) const
{
  m_actionMethod(&m_subscriber,
                 message); // [FIXED] was (m_subscriber.*ActionMethod)(message)
                           // -- ActionMethod is a type, not a value;
                           // std::function call syntax is different
}

template <typename Msg, typename Subscriber>
void Callback<Msg, Subscriber>::NotifyDisconnect()
{
  ICallback<Msg>::NotifyDisconnect(); // [ADDED] critical -- flips m_isConnected
                                      // in base
  if (m_disconnectMethod) // [ADDED] guard -- disconnect is optional (defaults
                          // to nullptr)
  {
    m_disconnectMethod(
        &m_subscriber); // [FIXED] was (m_subscriber.*DisconnectMethod)()
  }
}

} // namespace hrd42
#endif // MSG_BROKER_ILRD_HRD42_HPP