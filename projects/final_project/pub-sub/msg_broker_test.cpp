#include <chrono>
#include <iostream>
#include <memory> // shared_ptr, make_shared
#include <string>
#include <thread>

#include "msg_broker.hpp"

using namespace hrd42;

template <typename Msg> class Publisher
{
public:
  Publisher() : m_dispatcher(new Dispatcher<Msg>)
  {
  }
  ~Publisher()
  {
    m_dispatcher->NotifyDisconnect();

    delete m_dispatcher;
    m_dispatcher = nullptr;
  }

  Dispatcher<Msg>* GetDispatcher() const
  {
    return m_dispatcher;
  }

  void Disconnect() const
  {
    m_dispatcher->NotifyDisconnect();
  }

  void Publish()
  {
    size_t idx = 0;

    for (int i = 0; i < 3; i++)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      m_dispatcher->NotifyAll(std::string("Message num.") +
                              std::to_string(idx++));
    }
  }

private:
  Dispatcher<Msg>* m_dispatcher;
};

template <typename Msg> class Subscriber
{
public:
  Subscriber(Dispatcher<Msg>* dispatcher)
      : m_callback(new Callback<Msg, Subscriber>(
            dispatcher, *this, &Subscriber::Action, &Subscriber::Disconnect)),
        m_id(++s_id)
  {
  }
  ~Subscriber()
  {
    if (nullptr != m_callback)
    {
      OutOfService();
    }
  }

  void Action(const Msg& message)
  {
    std::cout << "Subscriber" << m_id << ": " << message << std::endl;
  }

  void Disconnect()
  {
    std::cout << "Subscriber" << m_id << ": publisher is out of service"
              << std::endl;
  }

  void OutOfService()
  {
    std::cout << "Subscriber" << m_id << " disconnecting" << std::endl;

    delete m_callback;
    m_callback = nullptr;
  }

private:
  Callback<Msg, Subscriber>* m_callback;
  size_t m_id;
  static size_t s_id;
};

template <typename Msg> size_t Subscriber<Msg>::s_id = 0;

int main()
{
  Dispatcher<std::string> dispatcher;

  Publisher<std::string> publisher;
  Subscriber<std::string> subscriber1(publisher.GetDispatcher());
  Subscriber<std::string> subscriber2(publisher.GetDispatcher());

  publisher.Publish();
  subscriber1.OutOfService();

  publisher.Publish();
  publisher.Disconnect();

  try
  {
    Subscriber<std::string> subscriber3(publisher.GetDispatcher());
  }
  catch (const std::runtime_error& e)
  {
    std::cout << "Subscription failed, published is disconnected" << std::endl;
  }

  return 0;
}