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
  struct sockaddr_in server_addr = {0};
  socklen_t server_len = sizeof(server_addr);

  /* create UDP socket */
  /* 0 =  choose protocol automatically (IPPROTO_UDP) */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  EXIT_IF_BAD_PERROR(-1 != sockfd, 1, "socket failed");

  /* fill my address */
  /* bind ourselves to a fixed port so the pong server can reply to us */
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(8001); /* our port */
  my_addr.sin_addr.s_addr = INADDR_ANY;

  /* bind  */
  EXIT_IF_BAD_PERROR(
      -1 != bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)), 1,
      "bind failed");

  /* fill server address */
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  while (1)
  {
    /* sendto - send ping to server */
    bytes = sendto(sockfd, "ping", 4, 0, (struct sockaddr*)&server_addr,
                   server_len);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "sendto failed");

    printf("Sent: ping\n");

    /* recvfrom - wait for pong back */
    bytes = recvfrom(sockfd, buf, BUF_SIZE - 1, 0,
                     (struct sockaddr*)&server_addr, &server_len);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recvfrom failed");

    buf[bytes] = '\0';
    printf("Received: %s\n", buf);

    sleep(1);
  }

  return 0;
}