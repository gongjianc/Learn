#include "head.h"

void send_n(int sfd,char* buf,int len)
{
	int total = 0;
	int size = 0;
	while(total < len){
		size = send(sfd,buf + total,len - total,0);
		total += size;
	}
}
	
void recv_n(int sfd,char* buf,int len)
{
	int total = 0;
	ssize_t size = 0;

    /* while(total < len && (size = recv(sfd, buf, 4096, 0) > 0)){
        printf("%s", buf);
        bzero(buf, strlen(buf));
        total += size;
    } */

    do{
        size = recv(sfd, buf, 4096, 0);
        printf("%s", buf);
        bzero(buf, strlen(buf));
        total += size;
    }while( (total < len && size > 0) || (size == -1 && errno == EINTR));

    printf("\n\n==============total is %d\n", total);
    printf("\n===============================FINISH===============================\n");
}
	
