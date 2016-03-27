#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	if(argc != 2){
		printf("args error\n");
		return -1;
	}

	int fd;
	fd = open(argv[1],O_RDWR | O_CREAT);
	if(fd < 0){
		perror("open");
		return -1;
	}
	int ret;
	char buf[128];
	while(bzero(buf,sizeof(buf)),(ret = read(fd,buf,sizeof(buf))) > 0){
		write(STDOUT_FILENO,buf,ret - 1);
	}	

	off_t currpos;
	currpos = lseek(fd,0,SEEK_CUR);
	printf("%ld\n",currpos);
	return 0;
}
