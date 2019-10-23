#include <bits/stdc++.h>  
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>


// create coded message based on character
std::string generateCode (std::string fileIn, char sym)
{
  std::string code;
  for (int i = 1; i < fileIn.length(); i++)
  {
    if (fileIn[i] == sym)
      code.append("1");
    else
      code.append("0");
  }
  return code;
}

void fireman(int)
{
   while (waitpid(-1, NULL, WNOHANG) > 0)
      std::cout << "A child process ended" << std::endl;
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portNum, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  pid_t pid;
  if (argc < 2) 
  {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    printf("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portNum = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portNum);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    printf("ERROR on binding");
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);
  signal(SIGCHLD, fireman);
  while (1) 
  {
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0) 
        printf("ERROR on accept");
    pid = fork();
    if (pid < 0)
      printf("ERROR creating process\n");
    if (pid == 0)
    {
      close(sockfd);
      bzero(buffer, 256);
      n = read(newsockfd, buffer, 255);
      if (n < 0) 
        printf("ERROR reading from socket");
      std::string msg = buffer;
      bzero(buffer, 256);
      msg = generateCode(msg, msg[0]);
      for (int i = 0; i < msg.length(); i++)
      {
        buffer[i] = msg[i];
      }
      n = write(newsockfd, buffer, 255);
      if (n < 0) 
        printf("ERROR writing to socket");
      exit(0);
    }
    else
    {
      close(newsockfd);
    }    
  }
  return 0; 
}
