#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char **argv)
{
	if(argc != 2){
		printf("error args\n");
		return -1;
	}

	int fds[2];
	pipe(fds); //创建管道
	/*子进程*/
	if(!fork()){
		close(fds[1]);
		int fd;
		read(fds[0],&fd,4);
		printf("I am child,fd is %d\n",fd);
		char buf[10] = {0};
		int ret;
		ret = read(fd,buf,sizeof(buf));
		if(-1 == ret){
			perror("read");
			return -1;
		}
		printf("%s\n",buf);
		exit(0);
	}else{
		close(fds[0]);
		int fd = open(argv[1],O_RDWR);
		if(-1 == fd){
			perror("open");
			return -1;
		}
		printf("I am parent,fd is %d\n",fd);
		write(fds[1],&fd,4);
		wait(NULL);
		exit(0);
	}
	return 0;
}
