#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h> 
#include <time.h>
#define buffersize 100

int main(int argc, char **argv)
{
    struct hostent *ip = gethostbyname(argv[1]);
    char buffer[buffersize];
    char message[] = "end";
    char get[] = "get";

    int sockfd,cliendfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
	struct sockaddr_in info;
	bzero(&info,sizeof(info));
	info.sin_family=AF_INET;
    bcopy((char *)ip->h_addr, (char *)&info.sin_addr.s_addr, ip->h_length);
	info.sin_port=htons(8080);

    FILE *fp=fopen("new.dat","wb");

    while(1){
        memset(buffer,0,buffersize);
        ssize_t by = recvfrom(sockfd, buffer, sizeof(buffer),0,(struct sockaddr *)&info, sizeof(info));
        if(strcmp(buffer,"end")==0){
            break;
        }
        fwrite(buffer,sizeof(char),by,fp);
    }
    
    fclose(fp);
    printf("end\n");
    close(sockfd);
	return 0;
}
