#include "icommand.hpp"
#include "threadpool.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

using namespace hrd42;

std::mutex g_print_mutex;

class CountCommand : public ICommand
{
public:
  CountCommand(std::atomic<int>& counter, Priority p = MID)
      : ICommand(p), m_counter(counter)
  {
  }
  void Execute() override
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ++m_counter;
  }
  std::atomic<int>& m_counter;
};

class PrintCountCommand : public CountCommand
{
public:
  PrintCountCommand(std::atomic<int>& counter, Priority p = MID)
      : CountCommand(counter, p)
  {
  }
  void Execute() override
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock(g_print_mutex);
    ++m_counter;
    std::cout << m_counter.load() << std::endl;
  }
};

class OrderCommand : public ICommand
{
public:
  OrderCommand(std::atomic<int>& order, std::atomic<int>& my_order, Priority p)
      : ICommand(p), m_order(order), m_my_order(my_order)
  {
  }
  void Execute() override
  {
    m_my_order = m_order++;
  }

private:
  std::atomic<int>& m_order;
  std::atomic<int>& m_my_order;
};

void TestBasic();
void TestPriority();
void TestSuspendResume();
void TestSetSizeGrow();
void TestSetSizeShrink();
void TestForceStop();
void TestStopThenSetSize();

int main()
{
  TestBasic();
  TestPriority();
  TestSuspendResume();
  TestSetSizeGrow();
  TestSetSizeShrink();
  TestForceStop();
  TestStopThenSetSize();

  return 0;
}

// commands are working
void TestBasic()
{
  std::atomic<int> counter(0);
  ThreadPool tp(4);
  for (int i = 0; i < 20; ++i)
  {
    tp.AddCommand(std::make_shared<PrintCountCommand>(counter));
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  tp.Stop();
  assert(counter == 20);
  std::cout << "TestBasic passed\n";
}

// check priority of commands
void TestPriority()
{
  std::atomic<int> order(0);
  std::atomic<int> high_order(-1);
  std::atomic<int> low_order(-1);
  ThreadPool tp(1);

  tp.AddCommand(
      std::make_shared<OrderCommand>(order, low_order, ICommand::LOW));
  tp.AddCommand(
      std::make_shared<OrderCommand>(order, high_order, ICommand::HIGH));
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  tp.Stop();
  assert(high_order < low_order);
  std::cout << "TestPriority passed\n";
}

// suspend and resume
void TestSuspendResume()
{
  std::atomic<int> counter(0);
  ThreadPool tp(4);
  tp.Suspend();
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  for (int i = 0; i < 10; ++i)
  {
    tp.AddCommand(std::make_shared<CountCommand>(counter));
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  assert(counter == 0);
  tp.Resume();
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  tp.Stop();
  assert(counter > 0);
  std::cout << "TestSuspendResume passed\n";
}

// resize threads number - bigger
void TestSetSizeGrow()
{
  std::atomic<int> counter(0);
  ThreadPool tp(2);
  tp.SetSize(8);
  for (int i = 0; i < 40; ++i)
  {
    tp.AddCommand(std::make_shared<CountCommand>(counter));
  }
  tp.Stop();
  assert(counter > 0);
  std::cout << "TestSetSizeGrow passed\n";
}

// resize threads number - smaller
void TestSetSizeShrink()
{
  std::atomic<int> counter(0);
  ThreadPool tp(8);
  tp.SetSize(2);
  for (int i = 0; i < 20; ++i)
  {
    tp.AddCommand(std::make_shared<CountCommand>(counter));
  }
  tp.Stop();
  assert(counter > 0);
  std::cout << "TestSetSizeShrink passed\n";
}

// force stop
void TestForceStop()
{
  std::atomic<int> counter(0);
  ThreadPool tp(4);
  for (int i = 0; i < 100; ++i)
  {
    tp.AddCommand(std::make_shared<CountCommand>(counter));
  }
  tp.ForceStop(std::chrono::milliseconds(50));
  assert(counter < 100);
  std::cout << "TestForceStop passed\n";
}

//  restart threads after stop
void TestStopThenSetSize()
{
  std::atomic<int> counter(0);
  ThreadPool tp(4);
  tp.Stop();
  for (int i = 0; i < 20; ++i)
  {
    tp.AddCommand(std::make_shared<PrintCountCommand>(counter));
  }
  tp.SetSize(2);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  tp.Stop();
  assert(counter > 0);
  std::cout << "TestStopThenSetSize passed\n";
}