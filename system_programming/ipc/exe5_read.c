#include "utils.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 64

int main()
{
  int shm_id = 0;
  char* shm_ptr = NULL;
  int i = 0;

  key_t key = ftok("/tmp/msgqueue_key", 'B');
  EXIT_IF_BAD(-1 != key, 1, "ftok failed");

  shm_id = shmget(key, SHM_SIZE, 0666);
  EXIT_IF_BAD_PERROR(-1 != shm_id, 1, "shmget failed");

  for (i = 2; i <= 4; ++i)
  {
    if (0 == fork())
    {
      shm_ptr = (char*)shmat(shm_id, NULL, 0);
      EXIT_IF_BAD_PERROR((void*)-1 != shm_ptr, 1, "shmat failed");

      printf("P%d pid:%d read: %s\n", i, getpid(), shm_ptr);

      shmdt(shm_ptr);
      return 0;
    }
  }

  for (i = 0; i < 3; ++i)
  {
    wait(NULL);
  }

  /* delete shared memory */
  EXIT_IF_BAD(-1 != shmctl(shm_id, IPC_RMID, NULL), 1,
              "failed to delete shared memory");

  return 0;
}