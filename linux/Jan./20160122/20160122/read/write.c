#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[])
{
	if(argc !=2 ){
		printf("args error\n");
		return -1;
	}
	int fd;
	//	fd = open(argv[1],O_RDWR|O_CREAT);
	fd = open(argv[1],O_RDWR | O_APPEND,0600);
	if(-1 == fd){
		perror("open");
		printf("error num is %d\n",errno);
		return -1;
	}
	char buf[128] = "world";
	int ret;
	ret = write(fd,buf,strlen(buf));
	if(ret < 0){
		perror("write");
		close(fd);
		return -1;
	}	
	close(fd);
	return 0;
}
