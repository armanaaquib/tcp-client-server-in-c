#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>

#define PORT "8000"
#define IPCOMP(addr, n) ((addr >> (24 - 8 * n)) & 0xFF)

struct sockaddr_in get_serveraddr(void)
{
  struct sockaddr_in serveraddr;
  bzero(&serveraddr, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  serveraddr.sin_port = htons(atoi(PORT));

  return serveraddr;
}

int start_listening(const struct sockaddr_in serveraddr, int no_of_connections)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("ERROR opening socket");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
  {
    perror("ERROR on binding");
    exit(1);
  }

  listen(sockfd, no_of_connections);
  printf("Listening At: " PORT "\n");

  return sockfd;
}

void handle_request(int sockfd, const struct sockaddr_in addr)
{
  while (1)
  {
    char buffer[256];
    bzero(buffer, 256);

    recv(sockfd, buffer, 255, 0);

    printf("Got %s\n", buffer);
    printf("From %d.%d.%d.%d:%d\n\n", IPCOMP(addr.sin_addr.s_addr, 0), IPCOMP(addr.sin_addr.s_addr, 1), IPCOMP(addr.sin_addr.s_addr, 2), IPCOMP(addr.sin_addr.s_addr, 3), addr.sin_port);

    send(sockfd, "Got your response\n", 19, 0);
  }
}

int main(void)
{
  int sockfd = start_listening(get_serveraddr(), 5);

  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  bzero(&client_addr, client_addr_size);

  while (1)
  {
    int c_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);

    if (c_sockfd < 0)
    {
      perror("ERROR on accept");
      exit(1);
    }

    if (fork() == 0)
    {
      close(sockfd);
      handle_request(c_sockfd, client_addr);
    }
    else
    {
      close(c_sockfd);
    }

    bzero(&client_addr, client_addr_size);
  }

  return 0;
}
