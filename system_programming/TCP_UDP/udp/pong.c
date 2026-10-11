#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "utils.h"

#define BUF_SIZE 256
#define PORT 8000

int main()
{

  int sockfd;
  char buf[BUF_SIZE];
  ssize_t bytes = 0;
  struct sockaddr_in my_addr = {0};
  struct sockaddr_in client_addr = {0};
  socklen_t client_len = sizeof(client_addr);

  /* create UDP socket */
  /* 0 =  choose protocol automatically (IPPROTO_UDP) */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  EXIT_IF_BAD_PERROR(-1 != sockfd, 1, "socket failed");

  /* fill my address */
  /* ipv4 */
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;

  /* bind - reserve port */
  EXIT_IF_BAD_PERROR(
      -1 != bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)), 1,
      "bind failed");

  printf("Pong server listening\n");

  while (1)
  {
    /* recvfrom - block until a datagram arrives */
    bytes = recvfrom(sockfd, buf, BUF_SIZE - 1, 0,
                     (struct sockaddr*)&client_addr, &client_len);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recvfrom failed");

    buf[bytes] = '\0';
    printf("Received: %s\n", buf);

    /* sendto - reply pong */
    bytes = sendto(sockfd, "pong", 4, 0, (struct sockaddr*)&client_addr,
                   client_len);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "sendto failed");

    printf("Sent: pong\n");
  }

  return 0;
}