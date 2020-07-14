#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(5000);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    perror("ERROR opening socket");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("ERROR connecting");
    exit(1);
  }

  int n = write(sockfd, "GET / HTTP/1.0\nhost: localhost\nUser-Agent: arman/1.0\nAccept: */*\n\n", 71);
  if (n < 0)
  {
    perror("ERROR writing to socket");
    exit(1);
  }

  char buffer[256];
  bzero(buffer, 256);

  n = read(sockfd, buffer, 255);

  if (n < 0)
  {
    perror("ERROR reading from socket");
    exit(1);
  }

  printf("%s\n", buffer);

  return 0;
}
