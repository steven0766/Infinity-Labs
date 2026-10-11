#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PING_PATH "/tmp/fifo_ping"
#define PONG_PATH "/tmp/fifo_pong"
#define READ 0
#define WRITE 1

static void Ping(int fd_out, int fd_in);

int main()
{
  int fd_out = 0;
  int fd_in = 0;
  EXIT_IF_BAD(mkfifo(PING_PATH, 0666) == 0 || errno == EEXIST, 1,
              "mkfifo ping failed");
  EXIT_IF_BAD(mkfifo(PONG_PATH, 0666) == 0 || errno == EEXIST, 1,
              "mkfifo pong failed");

  fd_out = open(PING_PATH, O_WRONLY); // write
  EXIT_IF_BAD_PERROR(fd_out >= 0, 1, "open PING_PATH failed");

  fd_in = open(PONG_PATH, O_RDONLY); // read
  EXIT_IF_BAD_PERROR(fd_in >= 0, 1, "open PONG_PATH failed");

  Ping(fd_out, fd_in);

  return SUCCESS;
}

static void Ping(int fd_out, int fd_in)
{
  char* msg = "ping";
  int msg_len = strlen(msg);
  char buf[10];
  int bytes_read = 0;

  while (1)
  {
    sleep(3);

    EXIT_IF_BAD(msg_len == write(fd_out, msg, msg_len), 1, "write failed");

    bytes_read = read(fd_in, buf, sizeof(buf));
    EXIT_IF_BAD(0 < bytes_read, 1, "read failed");

    buf[bytes_read] = '\0';
    printf("pid:%d %s\n", getpid(), buf);
  }
}