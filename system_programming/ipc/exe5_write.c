#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 64

int main()
{
  int shm_id = 0;
  char* shm_ptr = NULL;

  key_t key = ftok("/tmp/msgqueue_key", 'B');
  EXIT_IF_BAD(-1 != key, 1, "ftok failed");

  shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
  EXIT_IF_BAD_PERROR(-1 != shm_id, 1, "shmget failed");

  shm_ptr = (char*)shmat(shm_id, NULL, 0);
  EXIT_IF_BAD_PERROR((void*)-1 != shm_ptr, 1, "shmat failed");

  strcpy(shm_ptr, "hello from P1");
  printf("P1 pid:%d wrote: %s\n", getpid(), shm_ptr);

  shmdt(shm_ptr);

  return 0;
}