#include "utils.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define MAX_CLIENTS 10
#define PORT 8000
#define BACKLOG 5
#define TIMEOUT_SEC 7

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static void Log(const char* msg);
static void HandleStdin(void);
static void HandleUdp(int udp_fd);
static void HandleTcpListen(int tcp_listen_fd, int client_fds[],
                            size_t* num_clients, int* max_fd);
static void HandleTcpClient(int* client_fd, int client_fds[],
                            size_t* num_clients, fd_set* read_fds);

int main()
{
  int max_fd = 0;
  int num_clients = 0;
  int client_fds[MAX_CLIENTS];
  fd_set read_fds;
  struct timeval timeout = {0};
  int tcp_listen_fd = 0;
  int udp_fd = 0;
  int status = 0;
  int enable = 1;
  size_t i = 0;
  struct sockaddr_in my_addr = {0};

  /* init clients array */
  memset(client_fds, -1, sizeof(client_fds));

  /* create TCP listen socket */
  tcp_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  EXIT_IF_BAD_PERROR(-1 != tcp_listen_fd, 1, "tcp socket failed");

  /* allow reuse of port */
  EXIT_IF_BAD_PERROR(0 == setsockopt(tcp_listen_fd, SOL_SOCKET, SO_REUSEADDR,
                                     &enable, sizeof(enable)),
                     1, "setsockopt failed");

  /* create UDP socket */
  udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
  EXIT_IF_BAD_PERROR(-1 != udp_fd, 1, "udp socket failed");

  /* fill address */
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;

  /* bind TCP */
  EXIT_IF_BAD_PERROR(
      -1 != bind(tcp_listen_fd, (struct sockaddr*)&my_addr, sizeof(my_addr)), 1,
      "tcp bind failed");

  /* bind UDP */
  EXIT_IF_BAD_PERROR(
      -1 != bind(udp_fd, (struct sockaddr*)&my_addr, sizeof(my_addr)), 1,
      "udp bind failed");

  /* listen for TCP connections */
  EXIT_IF_BAD_PERROR(-1 != listen(tcp_listen_fd, BACKLOG), 1,
                     "tcp listen failed");

  /* set initial max_fd */
  max_fd = MAX(tcp_listen_fd, MAX(udp_fd, STDIN_FILENO));

  printf("Server listening on port %d\n", PORT);

  while (1)
  {
    /* rebuild fd set every iteration */
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(tcp_listen_fd, &read_fds);
    FD_SET(udp_fd, &read_fds);

    /* add all active TCP clients */
    for (i = 0; i < (size_t)num_clients; ++i)
    {
      if (-1 != client_fds[i])
      {
        FD_SET(client_fds[i], &read_fds);
      }
    }

    /* reset timeout */
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    /* block */
    do
    {
      status = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
      EXIT_IF_BAD((-1 != status) || (EINTR == errno), 1, "select failed");
    } while (-1 == status && EINTR == errno);

    /* nothing happened */
    if (0 == status)
    {
      Log("Nothing happened for 7 seconds");
      continue;
    }

    /* check stdin */
    if (FD_ISSET(STDIN_FILENO, &read_fds))
    {
      HandleStdin();
    }

    /* check UDP */
    if (FD_ISSET(udp_fd, &read_fds))
    {
      HandleUdp(udp_fd);
    }

    /* check TCP */
    if (FD_ISSET(tcp_listen_fd, &read_fds))
    {
      HandleTcpListen(tcp_listen_fd, client_fds, (size_t*)&num_clients,
                      &max_fd);
    }

    /* check TCP clients */
    for (i = 0; i < (size_t)num_clients; ++i)
    {
      if (-1 != client_fds[i] && FD_ISSET(client_fds[i], &read_fds))
      {
        HandleTcpClient(&client_fds[i], client_fds, (size_t*)&num_clients,
                        &read_fds);
      }
    }
  }

  return 0;
}

static void Log(const char* msg)
{
  printf("[LOG] %s\n", msg);
}

static void HandleStdin(void)
{
  char buf[BUFFER_SIZE];
  ssize_t bytes = 0;

  bytes = read(STDIN_FILENO, buf, BUFFER_SIZE - 1);
  EXIT_IF_BAD_PERROR(-1 != bytes, 1, "read stdin failed");

  buf[bytes] = '\0';

  /* strip newline */
  if (0 < bytes && '\n' == buf[bytes - 1])
  {
    buf[bytes - 1] = '\0';
  }

  if (0 == strcmp(buf, "ping"))
  {
    printf("pong\n");
  }
  else if (0 == strcmp(buf, "quit"))
  {
    Log("Server shutting down");
    exit(0);
  }
  else
  {
    Log("Unknown command");
  }
}

static void HandleUdp(int udp_fd)
{
  char buf[BUFFER_SIZE];
  ssize_t bytes = 0;
  struct sockaddr_in client_addr = {0};
  socklen_t client_len = sizeof(client_addr);
  const char* response = "pong";

  /* recvfrom and gives sender address  */
  bytes = recvfrom(udp_fd, buf, BUFFER_SIZE - 1, 0,
                   (struct sockaddr*)&client_addr, &client_len);
  EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recvfrom failed");

  buf[bytes] = '\0';
  printf("[UDP] Received: %s\n", buf);

  /* sendto - reply */
  bytes = sendto(udp_fd, response, strlen(response), 0,
                 (struct sockaddr*)&client_addr, client_len);
  EXIT_IF_BAD_PERROR(-1 != bytes, 1, "sendto failed");

  printf("[UDP] Sent: %s\n", response);
}

static void HandleTcpListen(int tcp_listen_fd, int client_fds[],
                            size_t* num_clients, int* max_fd)
{
  int client_fd = 0;
  struct sockaddr_in client_addr = {0};
  socklen_t client_len = sizeof(client_addr);

  client_fd =
      accept(tcp_listen_fd, (struct sockaddr*)&client_addr, &client_len);
  EXIT_IF_BAD_PERROR(-1 != client_fd, 1, "accept failed");

  printf("[TCP] Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr),
         ntohs(client_addr.sin_port));

  /* check if we have room */
  if (*num_clients >= MAX_CLIENTS)
  {
    Log("Max clients reached, rejecting connection");
    close(client_fd);
    return;
  }

  /* add to clients array */
  client_fds[*num_clients] = client_fd;
  ++(*num_clients);

  /* update max_fd */
  *max_fd = MAX(*max_fd, client_fd);
}

static void HandleTcpClient(int* client_fd, int client_fds[],
                            size_t* num_clients, fd_set* read_fds)
{
  char buf[BUFFER_SIZE];
  ssize_t bytes = 0;
  const char* response = "pong";

  bytes = recv(*client_fd, buf, BUFFER_SIZE - 1, 0);
  EXIT_IF_BAD_PERROR(-1 != bytes, 1, "recv failed");

  /* client disconnected */
  if (0 == bytes)
  {
    printf("[TCP] Client fd=%d disconnected\n", *client_fd);

    FD_CLR(*client_fd, read_fds);
    close(*client_fd);

    /* swap with last client */
    --(*num_clients);
    *client_fd = client_fds[*num_clients];
    client_fds[*num_clients] = -1;

    return;
  }

  buf[bytes] = '\0';
  printf("[TCP] Received: %s\n", buf);

  bytes = send(*client_fd, response, strlen(response), 0);
  EXIT_IF_BAD_PERROR(-1 != bytes, 1, "send failed");

  printf("[TCP] Sent: %s\n", response);
}