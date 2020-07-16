#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define HOST_ADDR "127.0.0.1"
#define PORT "8000"

struct sockaddr_in get_serveraddr(void)
{
  struct sockaddr_in serveraddr;
  bzero(&serveraddr, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(HOST_ADDR);
  serveraddr.sin_port = htons(atoi(PORT));

  return serveraddr;
}

int create_connection(const struct sockaddr_in serveraddr)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
  {
    perror("ERROR opening socket");
    exit(1);
  }

  if (connect(sockfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
  {
    perror("Could not connect to Server at " HOST_ADDR ":" PORT);
    return -1;
  }

  return sockfd;
}

int main(void)
{
  int sockfd = create_connection(get_serveraddr());

  while (1)
  {
    int buffer_size = 255;
    char buffer[buffer_size];
    bzero(buffer, buffer_size);

    printf("Enter your name: ");
    gets(buffer);

    send(sockfd, buffer, strlen(buffer), 0);

    bzero(buffer, buffer_size);
    recv(sockfd, buffer, buffer_size - 1, 0);

    printf("%s\n", buffer);
  }

  return 0;
}
