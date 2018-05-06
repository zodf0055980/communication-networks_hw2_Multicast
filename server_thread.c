#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> 

#define buffersize 256
#define hambuffersize 384

char max[]={0x03,0x05,0x06,0x07,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x11,0x12,0x13,0x14,0x15};
void *sender(void *);
int hamencode(char *,char *,long);
char *path;
int main (int argc, char **argv)
{
    char *path = argv[1];
    pthread_t thread_id;
    int sockfd,clientfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serverinfo,clientinfo;
	bzero (&serverinfo,sizeof(serverinfo));
	serverinfo.sin_family=AF_INET;
	serverinfo.sin_addr.s_addr=INADDR_ANY;
	serverinfo.sin_port=htons(8080);
	bind(sockfd,(struct sockaddr*)&serverinfo,sizeof(serverinfo));

	listen(sockfd,7);
	socklen_t clen;
    while(clientfd=accept(sockfd,(struct sockaddr*)&clientinfo,&clen)){
        pthread_create(&thread_id,NULL,sender,(void*) &clientfd);
        pthread_join( thread_id , NULL);
    }
	return 0;
}

void *sender(void *socket_desc) {
    int sock = *(int*)socket_desc;
    FILE *fp=fopen(path,"rb");
    char buffer[buffersize];
    char hambuffer[hambuffersize];
	char message[] = "end";
    int rb;
	int hamb;
    memset(buffer,0,buffersize);
    memset(hambuffer,0,hambuffersize);
    while(rb=fread(buffer,sizeof(char),sizeof(buffer),fp)) {
        hamb=hamencode(buffer,hambuffer,rb);
		send(sock, hambuffer, hamb, 0);
        memset(buffer,0,buffersize);
        memset(hambuffer,0,hambuffersize);
        usleep(100);
    }
    send(sock, message, sizeof(message), 0);
    fclose(fp);
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
		for(i = 0x80; i>0 ; i>>=1) {
			if(*(buf+ori) & i) {
				q = q ^ max[j];
			} 
			j++;
		}

		*(hbuf+ham)=*(buf+ori);
		ham++;
		ori++;

		for(i = 0x80; i>0 ; i>>=1) {
			if (*(buf+ori) & i) {
				q = q ^ max[j];
			} 
			j++;
		}

		*(hbuf+ham)=*(buf+ori);
		ham++;
		ori++;
		*(hbuf+ham) = q;
		ham++;
	}
	if(unham) {
			*(hbuf+ham)=*(buf+ori);
			ham++;
	}
    return ham;
}
