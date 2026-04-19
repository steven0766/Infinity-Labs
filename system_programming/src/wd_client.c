#define _POSIX_SOURCE

#include <stdio.h>     /* printf */
#include <sys/types.h> /* pid_t */
#include <unistd.h>    /* getpid, sleep */

#include "utils.h"
#include "wd.h"

int main(int argc, char* argv[])
{
  int status = 0;
  int num = 0;

  /* make immortal */
  status = MakeMeImmortal((size_t)argc, (const char**)argv, 1);
  EXIT_IF_BAD(SUCCESS == status, 1, "MakeMeImmortal failed");

  printf("PID: %d\n", getpid());

  while (1)
  {
    sleep(1);
    ++num;

    if (10 == num)
    {
      printf("DoNotResussitate activated\n");
      DoNotResussitate();
    }
  }

  return 0;
}
