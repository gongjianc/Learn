#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	if(argc !=2 ){
		printf("args error\n");
		return -1;
	}
	int fd;
	//	fd = open(argv[1],O_RDWR|O_CREAT);
	fd = open(argv[1],O_RDWR,0666);
	if(-1 == fd){
		perror("open");
		printf("error num is %d\n",errno);
		return -1;
	}
	int fd1;
	fd1 = dup(fd);
	if( -1 == fd1){
		perror("dup");
		close(fd);
		return -1;
	}	
	close(fd);
	char buf[128];
	bzero(buf,sizeof(buf));
	read(fd1,buf,sizeof(buf) - 1);	
	printf("the buf is %s\n",buf);
	close(fd1);
	return 0;
}
