#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>

int main(void)
{
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8000);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("ERROR opening socket");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("ERROR on binding");
    exit(1);
  }

  listen(sockfd, 5);

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
