#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 256
#define PORT 9000

int main()
{
  int sockfd;
  char buf[BUF_SIZE];
  ssize_t bytes = 0;
  struct sockaddr_in my_addr = {0};
  struct sockaddr_in sender_addr = {0};
  socklen_t sender_len = sizeof(sender_addr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  EXIT_IF_BAD_PERROR(-1 != sockfd, 1, "socket failed");

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;

  EXIT_IF_BAD_PERROR(
      -1 != bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)), 1,
      "bind failed");

  printf("Listening for broadcasts on port %d\n", PORT);

  while (1)
  {
    bytes = recvfrom(sockfd, buf, BUF_SIZE - 1, 0,
                     (struct sockaddr*)&sender_addr, &sender_len);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recvfrom failed");

    buf[bytes] = '\0';
    printf("Received from %s: %s\n", inet_ntoa(sender_addr.sin_addr), buf);
  }

  return 0;
}