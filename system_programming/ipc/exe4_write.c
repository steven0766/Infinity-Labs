#include "utils.h"
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

  q_id = msgget(key, IPC_CREAT | 0666);
  EXIT_IF_BAD_PERROR(-1 != q_id, 1, "msgget failed");

  for (i = 1; i <= 3; ++i)
  {
    msg.mtype = i;
    sprintf(msg.mtext, "msg%d", i);
    EXIT_IF_BAD_PERROR(0 == msgsnd(q_id, &msg, sizeof(msg.mtext), 0), 1,
                       "msgsnd failed");
  }

  return 0;
}