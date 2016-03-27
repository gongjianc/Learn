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
	fd = open(argv[1],O_RDWR,0600);
	if(-1 == fd){
		perror("open");
		printf("error num is %d\n",errno);
		return -1;
	}
	char buf[128] = "world";
	int ret;
	ret = lseek(fd,5,SEEK_SET);
	if(-1 == ret){
		perror("lseek");
		close(fd);
		return -1;
	}
	ret = write(fd,buf,strlen(buf));
	if(ret < 0){
		perror("write");
		close(fd);
		return -1;
	}	
	close(fd);
	return 0;
}
