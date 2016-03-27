#include <stdio.h>
#include <unistd.h>
#include <strings.h>

int main(void)
{
	sleep(8);
	int ret;
	char buf[128];
	while(bzero(buf,sizeof(buf)),(ret = read(STDIN_FILENO,buf,sizeof(buf))) > 0){
		printf("the buf is %s\n",buf);
	}
	return 0;
}		
