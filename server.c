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
    char buffer[buffersize];
    char message[] = "end";
    char get[] = "get";

    int sockfd,cliendfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serverInfo,clientInfo;
    bzero((char * )&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8080);
    socklen_t addrlen;
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));

    struct sockaddr_in  aad;
    socklen_t aadlen = sizeof(aad);

    FILE *fp=fopen(argv[1],"rb");
    fseek(fp,0,SEEK_END);
    long passsize=ftell(fp);
    fseek(fp,0,SEEK_SET);
    long count = (passsize/buffersize) + 1;
    passsize %= buffersize;

    int b;
    for(b=count;b>0;b--){
        memset(buffer,'\0',buffersize);
        if(b == 1) {
            fread(buffer,sizeof(char),passsize,fp);
            sendto(sockfd, buffer, passsize,0,(struct sockaddr *)&aad,&aadlen);
            sendto(sockfd, message, sizeof(message),0,(struct sockaddr *)&aad,&aadlen);
        } else {
            fread(buffer,sizeof(char),sizeof(buffer),fp);
            sendto(sockfd, buffer, sizeof(buffer),0,(struct sockaddr *)&aad,&aadlen);
        }
    }
    fclose(fp);
    printf("end\n");
    close(sockfd);
	return 0;
}
