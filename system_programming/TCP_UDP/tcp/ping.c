#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 256
#define PORT 8000

int main()
{
  int sockfd;
  char buf[BUF_SIZE];
  ssize_t bytes = 0;
  struct sockaddr_in server_addr = {0};

  /* create TCP socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  EXIT_IF_BAD_PERROR(-1 != sockfd, 1, "socket failed");

  /* fill server address */
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  /* connect - 3-way handshake, "locks" sockfd to server */
  EXIT_IF_BAD_PERROR(-1 != connect(sockfd, (struct sockaddr*)&server_addr,
                                   sizeof(server_addr)),
                     1, "connect failed");

  while (1)
  {
    /* send - no address needed, sockfd is connected */
    bytes = send(sockfd, "ping", 4, 0);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "send failed");
    printf("Sent: ping\n");

    /* recv - wait for pong back */
    bytes = recv(sockfd, buf, BUF_SIZE - 1, 0);
    EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recv failed");

    if (0 == bytes)
    {
      printf("Server disconnected\n");
      break;
    }

    buf[bytes] = '\0';
    printf("Received: %s\n", buf);

    sleep(1);
  }

  close(sockfd);
  return 0;
}