namespace useless
{
unsigned int g_wasteful[400];
extern int h_notthere;
void Foo()
{
}

namespace wasteoftyme
{
void Foo() {};
} // namespace wasteoftyme
} // namespace useless

namespace stupid
{
void Foo()
{
}
void Bar()
{
}
void DoNothing(unsigned int)
{
}
} // namespace stupid

namespace useless
{
void DoNothing(int)
{
}
} // namespace useless

using namespace useless;

void DoStuff()
{
  stupid::Bar();
  ::Foo();
  // Foo(); // Foo found 3 Foo's funcitons to choose from,
  using stupid::Foo;
  Foo();                        // stupid's Foo function
  DoNothing(g_wasteful[3] + 1); // using stupid's function
}

void Foo()
{
}

using namespace stupid;

namespace comeon = useless::wasteoftyme;

void DoMoreStuff()
{
  comeon::Foo();               // useless::wasteoftyme::Foo function
  useless::wasteoftyme::Foo(); // try uncommenting this line, solve the error
  Bar(); // why doesn;t this line create an error? "using namespace stupid"
         // allow us to use it
  DoNothing(g_wasteful[3] + 1); // using stupid function
}

namespace useless
{
void DoUselessStuff()
{
  DoNothing(g_wasteful[3] + 1);
}
} // namespace useless