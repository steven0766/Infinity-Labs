#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

struct BadDog : public runtime_error
{
  BadDog(const string& s_ = "This is a bad dog") : runtime_error(s_)
  {
  }
};

struct X
{
  X()
  {
    cerr << "X constructor" << endl;
  }
  ~X()
  {
    cerr << "X destructor" << endl;
  }
};

void Fifi()
{
  throw BadDog("bad pup");
  cerr << "Fifi() after throw" << endl;
}

void Foo()
{
  X x1;
  Fifi();
  cerr << " Foo() after Fifi()" << endl;
}

void Bar()
{
  Foo();
  cerr << "Bar() after Foo()" << endl;
}

int main()
{
  try
  {
    Bar();
  }
  catch (bad_alloc&)
  {
    cerr << "Out of memory! exiting.";
    exit(2);
  }
  //   catch (BadDog b)
  //   {
  //     cerr << "Bad dog exception: " << b.what();
  //     exit(2);
  //   }
  catch (exception r)
  {
    cerr << "unknown exception: " << r.what();
    exit(4);
  }
  catch (...)
  {
    cerr << "caught something unknown!" << endl;
  }

  return 0;
}