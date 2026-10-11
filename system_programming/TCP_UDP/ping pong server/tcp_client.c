#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 256
#define PORT 8000
#define MIN_MESSAGES 3
#define MAX_MESSAGES 10
#define MIN_SLEEP 3
#define MAX_SLEEP 10

static size_t GenerateRandom(size_t min, size_t max);

int main()
{
  int sockfd = 0;
  char buf[BUF_SIZE];
  ssize_t bytes = 0;
  struct sockaddr_in server_addr = {0};
  size_t messages = 0;
  size_t i = 0;

  srand(time(NULL));
  messages = GenerateRandom(MIN_MESSAGES, MAX_MESSAGES);

  /* create TCP socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  EXIT_IF_BAD_PERROR(-1 != sockfd, 1, "socket failed");

  /* fill server address */
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  /* connect */
  EXIT_IF_BAD_PERROR(-1 != connect(sockfd, (struct sockaddr*)&server_addr,
                                   sizeof(server_addr)),
                     1, "connect failed");

  printf("[TCP] Connected to server, sending %lu messages\n", messages);

  for (i = 0; i < messages; ++i)
  {
    /* send ping */
    bytes = send(sockfd, "ping", 4, 0);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "send failed");
    printf("[TCP] Sent: ping\n");

    /* wait for pong */
    bytes = recv(sockfd, buf, BUF_SIZE - 1, 0);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recv failed");

    if (0 == bytes)
    {
      printf("[TCP] Server disconnected\n");
      break;
    }

    buf[bytes] = '\0';
    printf("[TCP] Received: %s\n", buf);

    /* sleep between messages */
    if (i < messages - 1)
    {
      size_t tts = GenerateRandom(MIN_SLEEP, MAX_SLEEP);
      printf("[TCP] Sleeping %lu seconds\n", tts);
      sleep(tts);
    }
  }

  close(sockfd);
  printf("[TCP] Disconnected\n");

  return 0;
}

static size_t GenerateRandom(size_t min, size_t max)
{
  return (rand() % (max - min + 1)) + min;
}