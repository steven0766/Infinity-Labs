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
  status = MakeMeImmortal((size_t)argc, argv, 1, 3);
  EXIT_IF_BAD(SUCCESS == status, 1, "MakeMeImmortal failed");

  printf("PID: %d\n", getpid());

  while (1)
  {
    ++num;

    if (10 == num)
    {
      printf("DoNotResuscitate activated\n");
      DoNotResuscitate();
    }

    sleep(1);
  }

  return 0;
}
