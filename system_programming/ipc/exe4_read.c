#include "utils.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct
{
  long mtype;
  char mtext[10];
} msgbuf_ty;

int main()
{
  int q_id = 0;
  int i = 0;
  msgbuf_ty msg = {0};

  key_t key = ftok("/tmp", 'A');
  EXIT_IF_BAD(-1 != key, 1, "ftok failed");

  q_id = msgget(key, 0666);
  EXIT_IF_BAD_PERROR(-1 != q_id, 1, "msgget failed");

  for (i = 1; i <= 3; ++i)
  {
    if (0 == fork())
    {
      EXIT_IF_BAD_PERROR(-1 != msgrcv(q_id, &msg, sizeof(msg.mtext), i, 0), 1,
                         "msgrcv failed");
      printf("pid: %d  msg: %s\n", getpid(), msg.mtext);
      return 0;
    }
  }

  for (i = 0; i < 3; ++i)
  {
    wait(NULL);
  }

  EXIT_IF_BAD(-1 != msgctl(q_id, IPC_RMID, NULL), 1,
              "failed to delete message queue");
  return 0;
}