#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h> 
#include <time.h>
#include <unistd.h>
#define localip "127.0.0.1"
#define buffersize 256
#define hambuffersize 384

char max[]={0x03,0x05,0x06,0x07,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x11,0x12,0x13,0x14,0x15};
char hammat[]={0x01,0x02,0x04,0x08,0x10};
char correct[]={0x00,0x00,0x00,0x80,0x00,0x40,0x20,0x10,
0x00,0x08,0x04,0x02,0x01,0x80,0x40,0x20,0x00,0x10,0x08,0x04,0x02,0x01};

int hamdecode(char *,char *,long);

int main(int argc, char **argv)
{
    char buffer[buffersize];
    char hambuffer[hambuffersize];
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in info;
	bzero(&info,sizeof(info));
	info.sin_family=AF_INET;
    info.sin_addr.s_addr=inet_addr(localip);
	info.sin_port=htons(8080);
    connect(sockfd,(struct sockaddr *)&info,sizeof(info));

    FILE *filep=fopen("new.jpg","wb");
    int byte;
    while(1) {
        memset(buffer,0,buffersize);
        memset(hambuffer,0,hambuffersize);
        size_t n = read(sockfd,hambuffer,sizeof(hambuffer));
        if(strcmp(hambuffer,"end")==0){
           break;
        }
        byte=hamdecode(buffer,hambuffer,n);
        fwrite(buffer,sizeof(char),byte,filep);
    }

    fclose(filep);
    close(sockfd);
	return 0;
}


int hamdecode(char *buf,char *hbuf,long sz) {
    int unde=sz%3;
    char q;
    int i;
    int j=0;
    int l;
    int ham=0;
    int ori=0;
    for(l=0;l<(sz-unde);l+=3) {
        q = 0x00;
        for(i = 0x80; i>0 ; i>>=1) {
            if(*(hbuf+ham) & i) {
                q = q ^ max[j];
            }
            j++;
        }

        for(i = 0x80; i>0 ; i>>=1) {
            if(*(hbuf+ham+1) & i) {
                q = q ^ max[j];
            }
            j++;
        }
        j=0;
        for(i = 0x01; i<0x11 ; i<<=1) {
            if(*(hbuf+ham+2) & i) {
                q = q ^ hammat[j];
            } 
            j++;
        }
        j=0;
 
        char dd;
        if(q<=0x0c && q>0x00){
            *(hbuf+ham) = *(hbuf+ham)^correct[q];
        } else if (q>0x0c) {
            *(hbuf+ham+1) = *(hbuf+ham+1)^correct[q];
        } 

        *(buf+ori)=*(hbuf+ham);
        ori++;
        ham++;
        *(buf+ori)=*(hbuf+ham);
        ori++;
        ham+=2;
    }

    if(unde){
        *(buf+ori)=*(hbuf+ham);
        ori++;
        ham++;
    }
    return ori;
}