#include "factory.hpp"
#include "singleton.hpp"
#include <iostream>
#include <string>

using namespace hrd42;

class Person
{
public:
  Person(int id_, std::string name_) : id(id_), name(name_)
  {
  }
  void Walk()
  {
    std::cout << "lalala" << std::endl;
  }
  virtual ~Person() = default;

private:
  int id;
  std::string name;
};

class Teacher : public Person
{
public:
  Teacher(int id_, std::string name_) : Person(id_, name_)
  {
  }
  void Teach()
  {
    std::cout << "teaching " << std::endl;
  }
};

class Manager : public Person
{
public:
  Manager(int id_, std::string name_) : Person(id_, name_)
  {
  }

  void Bark()
  {
    std::cout << "woof" << std::endl;
  }
};

using PersonFactory = Factory<Person, std::string, int, std::string>;

int main()
{
  PersonFactory* pf = Singleton<PersonFactory>::GetInstance();
  pf->Add("Manager", [](int id_, std::string name_)
          { return std::make_shared<Manager>(id_, name_); });
  pf->Add("Teacher", [](int id_, std::string name_)
          { return std::make_shared<Teacher>(id_, name_); });

  std::shared_ptr<Person> m = pf->Create("Manager", 12345, "shmuel");
  std::dynamic_pointer_cast<Manager>(m)->Bark();

  std::shared_ptr<Person> p = pf->Create("Teacher", 54321, "lea");
  std::dynamic_pointer_cast<Teacher>(p)->Teach();

  return 0;
}