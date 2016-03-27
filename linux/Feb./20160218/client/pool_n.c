#include "head.h"

void recv_n(int sfd,char* buf,int len)
{
	int total = 0;
	int size = 0;
	while(total < len){
		size = recv(sfd,buf + total,len - total,0);
		total = total + size;
	}
}
