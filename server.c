#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffersize 256
#define hambuffersize 384

char max[]={0x03,0x05,0x06,0x07,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x11,0x12,0x13,0x14,0x15};

int hamencode(char *,char *,long);

int main (int argc, char **argv)
{
    char buffer[buffersize];
    char hambuffer[hambuffersize];
    char *path = argv[1];
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

    FILE *fp=fopen(path,"rb");

	char send[buffersize] = "start!";
	sendto(sockfd, send, sizeof(send), 0, (struct sockaddr*)&groupSock, sizeof(groupSock));

	char message[] = "end";
    int rb;
	int hamb;
    while(rb=fread(buffer,sizeof(char),sizeof(buffer),fp)) {
        hamb=hamencode(buffer,hambuffer,rb);
		printf("hamb = %d\n",hamb);
		sendto(sockfd, hambuffer, hamb, 0, (struct sockaddr*)&groupSock, sizeof(groupSock));
        // sendto(sockfd, buffer, rb , 0, (struct sockaddr*)&groupSock, sizeof(groupSock));
        memset(buffer,0,buffersize);
        memset(hambuffer,'\0',hambuffersize);
    }
    sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)&groupSock, sizeof(groupSock));

    fclose(fp);
	return 0;
}

int hamencode(char *buf,char *hbuf,long sz) {
	int unham = sz%2;
	int l=0;
	int ham=0;
	int ori=0;
	char q;
	int i;
	int j;
	for(l=0;l<(sz-unham);l+=2) {
		q = 0x00;
		j=0;
		// printf("a1 == ");
		for(i = 0x80; i>0 ; i>>=1) {
			if(*(buf+ori) & i) {
				q = q ^ max[j];
				// printf("1");
			} 
			// else
			// 	printf("0");
			j++;
		}

		*(hbuf+ham)=*(buf+ori);
		ham++;
		ori++;

		// printf("\na2 == ");
		for(i = 0x80; i>0 ; i>>=1) {
			if (*(buf+ori) & i) {
				q = q ^ max[j];
				// printf("1");
			} 
			// else
			// 	printf("0");
			j++;
		}
		// printf("\nj=%d",j);
		// printf("\nha == ");
    	// for(i = 0x01; i<0x11 ; i<<=1) {
        //     if(q & i) {
        //         printf("1");
        //     } else
        //         printf("0");
        // }
        
		// printf("\n");

		*(hbuf+ham)=*(buf+ori);
		ham++;
		ori++;
		*(hbuf+ham) = q;
		ham++;
	}
	if(unham) {
			// printf("\nnot do ham\n");
			*(hbuf+ham)=*(buf+ori);
			ham++;
	}
    return ham;
}