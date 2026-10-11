#include "singleton.hpp"
#include <iostream>
#include <thread>
#include <vector>

using namespace hrd42;

class Test
{
public:
  Test(int a = 0) : m_a(a)
  {
  }
  ~Test()
  {
  }
  int Get_m_a()
  {
    return m_a;
  }
  friend std::ostream& operator<<(std::ostream& os, const Test& t)
  {
    os << t.m_a;

    return os;
  }

private:
  int m_a;
};

void Singleton_Test();
void Multi_Thread_test();
void func(int val);

int main()
{
  Singleton_Test();
  Multi_Thread_test();

  return 0;
}

void Singleton_Test()
{
  Test* test = Singleton<Test>::GetInstance(5);
  std::cout << *test << std::endl;
}

void Multi_Thread_test()
{
  int rand_num = 5;
  int num_of_threads = 5;
  std::vector<std::thread> threads;
  for (int i = 0; i < num_of_threads; ++i)
  {
    threads.emplace_back(func, rand_num--);
  }
  for (auto& t : threads)
  {
    t.join();
  }
}

void func(int val)
{
  Test* t = Singleton<Test>::GetInstance(val);
  std::cout << "addr=" << t << " val=" << *t << std::endl;
}