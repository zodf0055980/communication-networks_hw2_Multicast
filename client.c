#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h> 
#include <time.h>
#include <unistd.h>
#define buffersize 100

int main(int argc, char **argv)
{
    char buffer[buffersize];
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0) {
        perror("Opening datagram socket error");    
    } 

    int a = 1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char *)&a,sizeof(a)) <0) {
        perror("Setting SO_REUSEADDR error");    
    }
    
	struct sockaddr_in info;
	bzero(&info,sizeof(info));
	info.sin_family=AF_INET;
    info.sin_addr.s_addr = INADDR_ANY;
	info.sin_port=htons(8080);

    bind(sockfd,(struct sockaddr*)&info,sizeof(info));
    // FILE *fp=fopen("new.dat","wb");

    // int loop = 1;
    // setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
    mreq.imr_interface.s_addr = inet_addr("127.0.0.1");;
    setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char *)&mreq,sizeof(mreq));

    memset(buffer,0,buffersize);
    recvfrom(sockfd, buffer, sizeof(buffer),0,(struct sockaddr *)&info, sizeof(info));
    printf("get = %s",buffer);
        // ssize_t by = recvfrom(sockfd, buffer, sizeof(buffer),0,(struct sockaddr *)&info, sizeof(info));
        // if(strcmp(buffer,"end")==0){
        //     break;
        // }
        // fwrite(buffer,sizeof(char),by,fp);
 //       sleep(5000);
        // fclose(fp);

    setsockopt(sockfd,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
    printf("end\n");
    close(sockfd);
	return 0;
}
