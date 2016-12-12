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
#include <time.h>
#include <sys/time.h>

#define MAXLINE 1024 // NOTE: Can be modified to any size you want.


void main (int argc, char **argv)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  char recipient_address[MAXLINE]; // "RCPT TO" address
  char sender_address[MAXLINE]; // "MAIL FROM" address
  char *temp;
  char *Server_IP = argv[1];
  printf("%s", Server_IP);
  
  if (argc != 3)
    {
      printf("Error: Must enter a host name and port number \n");
      exit(0);
    }

  //Convert argv[1] to integer before comparing
  int portNum = atoi(argv[2]);
  //printf("%d \n", portNum);
  
  if(portNum <= 1024){
    printf("Error: Must enter a port number greater than 1024 \n");
    exit(0);
  }

  int fd;
  struct sockaddr_in myaddr;
  
  if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    printf("could not connect \n");
    exit(0);
  }

  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = inet_addr(Server_IP);
  myaddr.sin_port = htons(portNum);

  if (connect(fd, (struct sockaddr *)&myaddr, sizeof(myaddr))) {
    printf("connection failed \n");
    exit(0);
  }

  int lostPackets = 0; //Counts number of dropped/lost packets
  int rttTotal = 0;    //Contains total transmission time
  int maxTime = 0;     //Contains Max RTT
  int minTime = 999999;//Contains Min RTT
  int avgTime = 0;     //Contains avg RTT
  
  for(int i = 0; i < 10; i++){
    struct timeval tv, start, end;
    //int t1, t2 = 0;
    int rtt = 0;
    tv.tv_sec = 1;  //1 Sec Timeout 
    tv.tv_usec = 0; 
    
    char result[MAXLINE];

    gettimeofday(&start, NULL);
    write(fd, sendline, strlen(sendline));
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    
    //Add ping to the message
    sprintf(result, "PING seq = %d \n", i);
    //printf("%s", result);

    //Receive the data from the server
    ssize_t r = read(fd, recvline, MAXLINE);
    gettimeofday(&end, NULL);

    //Compute the rtt
    rtt = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
    rtt = rtt/1000;  //Convert to ms

    //Set max and min rtt times
    if(rtt > maxTime && rtt < 1000){
      maxTime = rtt;
    }
    if(rtt < minTime){
      minTime = rtt;
    }
    if(r < 0){
      lostPackets++;
    }

    else{
      //packet was received
      rttTotal += rtt;
      printf("PING received from %s seq#= %d time = %d ms\n", Server_IP, i, rtt);
    }
    avgTime = (rttTotal/(10 - lostPackets));
  }

  int receivedPackets = 10 - lostPackets;
  int percentLoss = lostPackets * 10;
  printf("--- ping statistics ---\n");
  printf("10 packets transmitted, %d packets received, %d %% packet loss rtt min/avg/max = %d %d %d ms \n", receivedPackets, percentLoss, minTime, avgTime, maxTime);
  return;
}
