#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PING_PATH "/tmp/fifo_ping"
#define PONG_PATH "/tmp/fifo_pong"

static void Pong(int fd_in, int fd_out);

int main()
{
  int fd_in = open(PING_PATH, O_RDONLY);
  int fd_out = 0;
  EXIT_IF_BAD_PERROR(fd_in >= 0, 1, "open PING_PATH failed");

  fd_out = open(PONG_PATH, O_WRONLY);
  EXIT_IF_BAD_PERROR(fd_out >= 0, 1, "open PONG_PATH failed");

  Pong(fd_in, fd_out);

  return SUCCESS;
}

static void Pong(int fd_in, int fd_out)
{
  char* msg = "pong";
  int msg_len = strlen(msg);
  char buf[10];
  int bytes_read = 0;

  while (1)
  {
    bytes_read = read(fd_in, buf, sizeof(buf));
    EXIT_IF_BAD(0 < bytes_read, 1, "read failed");

    buf[bytes_read] = '\0';
    printf("pid:%d %s\n", getpid(), buf);

    sleep(3);

    EXIT_IF_BAD(msg_len == write(fd_out, msg, msg_len), 1, "write failed");
  }
}