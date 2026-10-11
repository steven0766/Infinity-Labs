#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 256
#define PORT 8000
#define BACKLOG 5

int main()
{
  int listenfd, connfd;
  char buf[BUF_SIZE];
  ssize_t bytes = 0;
  struct sockaddr_in my_addr = {0};
  struct sockaddr_in client_addr = {0};
  socklen_t client_len = sizeof(client_addr);

  /* create TCP socket */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  EXIT_IF_BAD_PERROR(-1 != listenfd, 1, "socket failed");

  /* fill my address */
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;

  /* bind - reserve port 8000 on this machine */
  EXIT_IF_BAD_PERROR(
      -1 != bind(listenfd, (struct sockaddr*)&my_addr, sizeof(my_addr)), 1,
      "bind failed");

  /* listen - mark socket as passive, ready to accept connections */
  EXIT_IF_BAD_PERROR(-1 != listen(listenfd, BACKLOG), 1, "listen failed");

  printf("Pong server listening\n");
  while (1)
  {
    /* accept - block until a client connects */
    connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);
    EXIT_IF_BAD_PERROR(-1 != connfd, 1, "accept failed");

    printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    while (1)
    {
      /* recv - no address needed, connfd is "locked" to this client */
      bytes = recv(connfd, buf, BUF_SIZE - 1, 0);
      EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recv failed");

      if (0 == bytes)
      {
        printf("Client disconnected\n");
        break;
      }

      buf[bytes] = '\0';
      printf("Received: %s\n", buf);

      /* send reply */
      bytes = send(connfd, "pong", 4, 0);
      EXIT_IF_BAD_PERROR(-1 != bytes, 1, "send failed");
      printf("Sent: pong\n");
    }
  }
  return 0;
}
