#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 22 23 24 
0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1
X X 0 X 0 0 0 X 0 0 0 1 1 0 0 X 0 0 0 0 1
*/

char max[]={0x03,0x05,0x06,0x07,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x11,0x12,0x13,0x14,0x15};
char ham[]={0x01,0x02,0x04,0x08,0x10};

int main()
{
    char a1 = 0x41;
    char a2 = 0x81;
    char a3 = 0x14;
    char first = 0x01;
    char q = 0;
    char b;
    int i;
    int j=0;
    for(i = 0x80; i>0 ; i>>=1) {
        b = (a1 & i) ? '1': '0';
        if(b == '1') {
            q = q ^ max[j];
        }
        printf("%c",b);
        j++;
    }

    printf("\n");
    for(i = 0x80; i>0 ; i>>=1) {
        b = (a2 & i) ? '1': '0';
        if(b == '1') {
            q = q ^ max[j];
        }
        printf("%c",b);
        j++;
    }

    printf("\n");
    j=0;
    for(i = 0x01; i<0x11 ; i<<=1) {
        b = (a3 & i) ? '1': '0';
        if(b == '1') {
            q = q ^ ham[j];
        }
        printf("%c",b);
        j++;
    }

        printf("\n");
    j=0;
    for(i = 0x80; i>0 ; i>>=1) {
        b = (q & i) ? '1': '0';
        printf("%c",b);
        j++;
    }
}
/*
1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 
0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1
X X 0 X 0 0 0 X 0 0 0 1 1 0 0 X 0 0 0 0 1
*/