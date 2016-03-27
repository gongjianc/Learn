#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>


int main()
{
	pid_t pid;
	pid = fork();
	if(pid == 0){
		printf("my pid is %d,ppid = %d \n",getpid(),getppid());
		exit(0);
	}else{
		printf("my pid is %d,ppid = %d \n",getpid(),getppid());
		sleep(2);
		return 0;
	}
}
