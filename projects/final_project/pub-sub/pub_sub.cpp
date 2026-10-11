// Many-to-many Pub/Sub broker
// - Any number of publishers can publish to any topic
// - Any number of subscribers can subscribe to any number of topics
// - Publisher and Subscriber never reference each other, only Broker
// - RAII SubscriptionToken solves the dangling-callback problem from before

#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

struct Event
{
  std::string topic;
  std::string payload;
};

using Callback = std::function<void(const Event&)>;

class Broker; // fwd decl

// RAII handle: unsubscribes automatically on destruction (or moves ownership)
class SubscriptionToken
{
public:
  SubscriptionToken() = default;
  SubscriptionToken(Broker* broker, std::string topic, uint64_t id)
      : broker_(broker), topic_(std::move(topic)), id_(id)
  {
  }

  SubscriptionToken(const SubscriptionToken&) = delete;
  SubscriptionToken& operator=(const SubscriptionToken&) = delete;

  SubscriptionToken(SubscriptionToken&& other) noexcept
  {
    *this = std::move(other);
  }
  SubscriptionToken& operator=(SubscriptionToken&& other) noexcept
  {
    if (this != &other)
    {
      reset();
      broker_ = other.broker_;
      topic_ = std::move(other.topic_);
      id_ = other.id_;
      other.broker_ = nullptr;
    }
    return *this;
  }

  ~SubscriptionToken()
  {
    reset();
  }

  void reset(); // defined after Broker below

private:
  Broker* broker_ = nullptr;
  std::string topic_;
  uint64_t id_ = 0;
};

class Broker
{
public:
  // Any object, anywhere, can call this. No shared base class needed.
  SubscriptionToken subscribe(const std::string& topic, Callback cb)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    uint64_t id = next_id_++;
    topics_[topic].push_back({id, std::move(cb)});
    return SubscriptionToken(this, topic, id);
  }

  void unsubscribe(const std::string& topic, uint64_t id)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = topics_.find(topic);
    if (it == topics_.end())
      return;
    auto& subs = it->second;
    subs.erase(std::remove_if(subs.begin(), subs.end(),
                              [&](auto& s) { return s.id == id; }),
               subs.end());
  }

  // Any publisher, anywhere, can call this — same broker, different topic.
  void publish(const Event& e)
  {
    std::vector<Callback> snapshot;
    {
      // Copy callbacks under lock, then call outside the lock.
      // This avoids holding the mutex during arbitrary subscriber code,
      // and avoids the "subscriber unsubscribes itself mid-notify" bug.
      std::lock_guard<std::mutex> lock(mutex_);
      auto it = topics_.find(e.topic);
      if (it == topics_.end())
        return;
      for (auto& s : it->second)
        snapshot.push_back(s.cb);
    }
    for (auto& cb : snapshot)
      cb(e);
  }

private:
  friend class SubscriptionToken;
  struct Entry
  {
    uint64_t id;
    Callback cb;
  };

  std::mutex mutex_;
  std::unordered_map<std::string, std::vector<Entry>> topics_;
  std::atomic<uint64_t> next_id_{1};
};

void SubscriptionToken::reset()
{
  if (broker_)
  {
    broker_->unsubscribe(topic_, id_);
    broker_ = nullptr;
  }
}

// ---------------------------------------------------------------------
// Demo: 2 publishers, 3 subscribers, 2 topics -> genuine many-to-many
// ---------------------------------------------------------------------

class TemperatureSensor
{
public:
  TemperatureSensor(Broker& b, std::string name)
      : broker_(b), name_(std::move(name))
  {
  }
  void publishReading(double celsius)
  {
    broker_.publish({"temperature", name_ + ": " + std::to_string(celsius)});
  }

private:
  Broker& broker_;
  std::string name_;
};

class Logger
{
public:
  explicit Logger(Broker& b)
  {
    // Subscribes to TWO different topics with plain lambdas — no inheritance
    token1_ = b.subscribe(
        "temperature", [](const Event& e)
        { std::cout << "[Logger] temp event: " << e.payload << "\n"; });
    token2_ = b.subscribe(
        "alerts", [](const Event& e)
        { std::cout << "[Logger] alert event: " << e.payload << "\n"; });
  }

private:
  SubscriptionToken token1_, token2_;
};

class Dashboard
{
public:
  explicit Dashboard(Broker& b)
  {
    token_ = b.subscribe("temperature", [this](const Event& e) { onTemp(e); });
  }
  void onTemp(const Event& e)
  {
    std::cout << "[Dashboard] updating UI with: " << e.payload << "\n";
  }

private:
  SubscriptionToken token_;
};

int main()
{
  Broker broker;

  TemperatureSensor sensorA(broker, "SensorA");
  TemperatureSensor sensorB(broker, "SensorB");

  Logger logger(broker);
  Dashboard dashboard(broker);

  {
    // A third subscriber with a limited lifetime -- when it goes out of
    // scope, its SubscriptionToken destructor auto-unsubscribes.
    // No dangling-pointer callback, unlike raw Observer::attach/detach.
    auto tempToken = broker.subscribe(
        "temperature", [](const Event& e)
        { std::cout << "[TempWatcher-scoped] " << e.payload << "\n"; });

    sensorA.publishReading(
        21.5); // 2 subscribers still alive get this + tempToken
  } // tempToken destroyed here -> auto-unsubscribed

  sensorB.publishReading(19.0); // only logger + dashboard get this now
  broker.publish(
      {"alerts", "SensorA offline"}); // only logger subscribed to "alerts"

  return 0;
}