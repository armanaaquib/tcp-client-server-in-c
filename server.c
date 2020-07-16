#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>

#define PORT "8000"

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
  return sockfd;
}

int main(void)
{
  int sockfd = start_listening(get_serveraddr(), 2);

  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  int c_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);

  if (c_sockfd < 0)
  {
    perror("ERROR on accept");
    exit(1);
  }

  char buffer[256];
  bzero(buffer, 256);

  int n = recv(c_sockfd, buffer, 255, 0);
  if (n < 0)
  {
    perror("ERROR reading from socket");
    exit(1);
  }

  printf("%s\n", buffer);

  n = send(c_sockfd, "Hello", 5, 0);
  if (n < 0)
  {
    perror("ERROR writing to socket");
    exit(1);
  }

  return 0;
}
