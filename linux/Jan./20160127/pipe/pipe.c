#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 128

int
main(void)
{
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];

	if(pipe(fd) < 0){
		perror("pipe");
		return -1;
	}

	if((pid = fork()) < 0){
		perror("fork");
		return -1;
	}else if(pid > 0){
		close(fd[0]);
		write(fd[1],"hello world\n",12);
	}else{
		close(fd[1]);
		n = read(fd[0],line,MAXLINE);
		write(STDOUT_FILENO,line,n);
	}
	return 0;
}

