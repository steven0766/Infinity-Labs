#include <dlfcn.h>
#include <iostream>

int main()
{
  void* handle = dlopen("./libmylib.so", RTLD_LAZY);
  if (!handle)
  {
    std::cout << "handle fail" << std::endl;
  }

  void (*myfunc)(int, int);
  myfunc = (void (*)(int, int))dlsym(handle, "AddPrint");
  char* err = dlerror();
  if (NULL != err)
  {
    std::cout << "didn't find function" << std::endl;
    dlclose(handle);

    return 0;
  }
  myfunc(5, 7);
  dlclose(handle);

  return 0;
}