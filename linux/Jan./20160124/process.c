#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 100

int main(void)
{
	char buf[MAXLINE];
	pid_t pid;
	int status;

	printf("%%");
	while(fgets(buf,MAXLINE,stdin) != NULL){
		if(buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
			
		if((pid = fork()) < 0){
			perror("fork error");
		}else if(pid == 0){
			execlp(buf,buf,(char *)0);
			printf("couldn`t execute: %s",buf);
			exit(127);
		}

		if((pid = waitpid(pid,&status,0)) < 0)
			perror("waitpid error");
		printf("%%");

			
	}
	exit(0);
}
