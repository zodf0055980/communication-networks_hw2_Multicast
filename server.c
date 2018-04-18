#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define buffersize 256

int main (int argc, char **argv)
{
	struct in_addr localInterface;
	struct sockaddr_in groupSock;
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
	  perror("Opening datagram socket error");
	  exit(1);
	}
	 
	memset((char *) &groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(8080);
	
	localInterface.s_addr = inet_addr("127.0.0.1");
	if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	  perror("set interface error");
	  exit(1);
	}

    // FILE *fp=fopen(argv[1],"rb");
    // fseek(fp,0,SEEK_END);
    // long passsize=ftell(fp);
    // fseek(fp,0,SEEK_SET);
    // long count = (passsize/buffersize) + 1;
    // passsize %= buffersize;
		char message[buffersize] = "fuck!";
	if(sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
	{
		perror("send error");
	}

    // int b;
    // for(b=count;b>0;b--){
    //     memset(buffer,'\0',buffersize);
    //     if(b == 1) {
    //         fread(buffer,sizeof(char),passsize,fp);
    //         sendto(sockfd, buffer, passsize,0,(struct sockaddr *)&aad,&aadlen);
    //         sendto(sockfd, message, sizeof(message),0,(struct sockaddr *)&aad,&aadlen);
    //     } else {
    //         fread(buffer,sizeof(char),sizeof(buffer),fp);
    //         sendto(sockfd, buffer, sizeof(buffer),0,(struct sockaddr *)&aad,&aadlen);
    //     }
    // }
    // fclose(fp);
	return 0;
}

