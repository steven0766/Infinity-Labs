#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 256
#define PORT 9000
#define BROADCAST_ADDR "255.255.255.255"
#define NUM_MESSAGES 5

int main()
{
  int sockfd;
  int broadcast_enable = 1;
  struct sockaddr_in broadcast_addr = {0};
  const char* msg = "Hello from broadcast!";
  ssize_t bytes = 0;
  int i;

  /* create UDP socket */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  EXIT_IF_BAD_PERROR(-1 != sockfd, 1, "socket failed");

  /* enable broadcast permission on this socket */
  EXIT_IF_BAD_PERROR(-1 != setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
                                      &broadcast_enable,
                                      sizeof(broadcast_enable)),
                     1, "setsockopt failed");

  /* fill broadcast address */
  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_port = htons(PORT);
  broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR);

  /* send several messages with sleep */
  for (i = 0; i < NUM_MESSAGES; i++)
  {
    bytes = sendto(sockfd, msg, strlen(msg), 0,
                   (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "sendto failed");

    printf("Broadcast sent: %s\n", msg);
    sleep(1);
  }

  close(sockfd);
  return 0;
}