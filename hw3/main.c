/*
* CS 176A Programming Assignment 3
*
* The command line arguments should be:
* ./EmailSender <recipient address>
*
* NOTE:
* 1- I would like you to hardcode the following:
*       a- Sender address: "MAIL FROM".
*       b- Message you are sending.
* 2- Pass the recipient address "RCPT TO" as a command line argument.
* 3- INCLUDE comments in the code to show what is going on (not too
detailed).
* 4- INCLUDE a README file if something did not run properly. That will
help you receive partial credit.
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#define SMTP_PORT 25
#define MAXLINE 1024 // NOTE: Can be modified to any size you want.

char *Server_IP = "128.111.53.4"; // NOTE: IP of the mail server after running "nslookup -type=MX cs.ucsb.edu". Hence you will need to change this value. Changed to IP Address of mx1.engr.ucsb.edu which is 128.111.53.4

void main (int argc, char ** argv)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  char recipient_address[MAXLINE]; // "RCPT TO" address.
  char sender_address[MAXLINE]; // "MAIL FROM" adress.
  char *temp;
  if (argc != 2)
  {
    printf("usage: ./EmailSender <recipient address,\"RCPTTO\">\n");
    exit(0);
  }

  strcpy(recipient_address, argv[1]);

  /* Establish a TCP connection with the main server */
  int sockfd;
  int portNum = 25;

 //create an INET, STREAMing socket
  struct sockaddr_in serv_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  //Ensure the socket was created successfully
  if(sockfd == -1){
    printf("Could not create socket\n");
    exit(1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(Server_IP);
  serv_addr.sin_port = htons(portNum);

  //Ensure that the connection was made successfully
  if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))){
    printf("Could not connect\n");
    exit(1);
  }
    

  /* Read greeting from the server */
  read(sockfd, recvline, MAXLINE);
  printf("%s\n", recvline);
  temp = strtok(recvline, " ");
  if (strcmp(temp, "220")!=0)
  {
    printf("220 reply not received from server.\n");
    exit(0);
  }

  /* Send HELO command and get server response */
  strcpy(sendline, "HELO Kenneth\r\n");
  printf("%s\n", sendline);
  write(sockfd, sendline, strlen(sendline));

  read(sockfd, recvline, MAXLINE);
  printf("%s\n", recvline);
  temp = strtok(recvline, " ");
  if (strcmp(temp, "250")!=0)
  {
    printf("250 reply not received from server.\n");
    exit(0);
  }

  memset(sendline, 0, sizeof(sendline));
  memset(recvline, 0, sizeof(recvline));

  // Send MAIL FROM command.
  strcpy(sender_address, "MAIL FROM: kctoombs@umail.ucsb.edu\r\n"); // NOTE:replace address with your own.
  printf("%s\n", sender_address);
  int length = strlen(sender_address);
  write(sockfd, sender_address, strlen(sender_address));
  read(sockfd, recvline, 1024);
  printf("%s\n", recvline);
  temp = strtok(recvline, " ");

  if (strcmp(temp, "250")!=0)
    {
      printf("250 reply not received from server.\n");
      exit(0);
    }
  
  memset(recvline, 0, sizeof(recvline));
  

  // Send RCPT TO command.
  strcpy(sendline, "RCPT TO: <");
  strcat(sendline, recipient_address);
  strcat(sendline, ">\r\n");
  printf("%s\n", sendline);
  int lineLength = strlen(sendline);
  write(sockfd, sendline, lineLength);
  read(sockfd, recvline, 1024);
  printf("%s\n", recvline);
  temp = strtok(recvline, " ");

  if (strcmp(temp, "250")!=0)
    {
      printf("250 reply not received from server.\n");
      exit(0);
    }
  memset(recvline, 0, sizeof(recvline));
  
  // Send DATA command.
  strcpy(sendline, "DATA\r\n");
  printf("%s\n", sendline);
  int sendLength = strlen(sendline);
  write(sockfd, sendline, sendLength);
  read(sockfd, recvline, 1024);
  printf("%s\n", recvline);

  temp = strtok(recvline, " ");
  if (strcmp(temp, "354")!=0)
    {
      printf("354 did not receive reply from server.\n");
      exit(0);
    }
  memset(recvline, 0, sizeof(recvline));
  memset(sendline, 0, sizeof(sendline));

  // Send message data.
  strcpy(sendline, "SUBJECT: Hello!!\r\n\r\nHello there, friend\r\n");
  printf("%s\n", sendline);
  int sendLineLen = strlen(sendline);
  write(sockfd, sendline, sendLineLen);
  memset(sendline, 0, sizeof(sendline));
  
  // End with a single period.
  strcpy(sendline, ".\r\n");
  printf("%s\n", sendline);
  write(sockfd, sendline, strlen(sendline));
  memset(sendline, 0, sizeof(sendline));

  // Send QUIT command.
  strcpy(sendline, "QUIT\r\n");
  printf("%s\n", sendline);
  write(sockfd, sendline, strlen(sendline));
  read(sockfd, recvline, 1024);
  printf("%s\n", recvline);
  
  temp = strtok(recvline, " ");
  if (strcmp(temp, "250")!=0)
    {
      printf("250 reply not received from server.\n");
      exit(0);
    }

  close(sockfd); //Close the socket
  
  exit(0);
}
