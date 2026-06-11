#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

static void Ping(int p1[2], int p2[2]);
static void Pong(int p1[2], int p2[2]);

int main()
{
  pid_t pid = 0;
  int p1[2]; /* parent write to child */
  int p2[2]; /* child write to parent */

  EXIT_IF_BAD_PERROR(0 == pipe(p1), 1, "Failed to create pipe");
  EXIT_IF_BAD_PERROR(0 == pipe(p2), 1, "Failed to create pipe");

  pid = fork();
  EXIT_IF_BAD_PERROR(-1 != pid, 1, "fork failed");

  if (0 == pid)
  {
    Pong(p1, p2);
    return 0;
  }

  Ping(p1, p2);
  return 0;
}

static void Ping(int p1[2], int p2[2])
{
  char* msg = "ping";
  int msg_len = strlen(msg);
  char buf[10];
  int bytes_read = 0;

  EXIT_IF_BAD_PERROR(0 == close(p1[READ]), 1, "close failed");
  EXIT_IF_BAD_PERROR(0 == close(p2[WRITE]), 1, "close failed");

  while (1)
  {
    sleep(3);

    EXIT_IF_BAD(msg_len == write(p1[WRITE], msg, msg_len), 1, "write failed");

    bytes_read = read(p2[READ], buf, sizeof(buf));
    EXIT_IF_BAD(0 < bytes_read, 1, "read failed");

    buf[bytes_read] = '\0';
    printf("pid:%d %s\n", getpid(), buf);
  }
}

static void Pong(int p1[2], int p2[2])
{
  char* msg = "pong";
  int msg_len = strlen(msg);
  char buf[10];
  int bytes_read = 0;

  EXIT_IF_BAD_PERROR(0 == close(p1[WRITE]), 1, "close failed");
  EXIT_IF_BAD_PERROR(0 == close(p2[READ]), 1, "close failed");

  while (1)
  {
    bytes_read = read(p1[READ], buf, sizeof(buf));
    EXIT_IF_BAD(0 < bytes_read, 1, "read failed");

    buf[bytes_read] = '\0';
    printf("pid:%d %s\n", getpid(), buf);

    sleep(3);

    EXIT_IF_BAD(msg_len == write(p2[WRITE], msg, msg_len), 1, "write failed");
  }
}