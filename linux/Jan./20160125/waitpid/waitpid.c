#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	pid = fork();
	if(pid == 0){
		printf("this is child\n");
		while(1);
	}else{
		int status;
		int ret;
		ret = waitpid(pid,&status,WNOHANG);
		printf("the ret is %d\n",ret);
		if(ret < 0){
			perror("waitpid");
			return -1;
		}
		if(WIFEXITED(status)){
			printf("the exit value is %d\n",WEXITSTATUS(status));
		}
		return 0;
	}
}

