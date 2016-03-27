#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	pid = fork();

	if( pid > 0){

		printf("i am parent\n");	
		printf("my info: pid = %d,ppid = %d,pgid = %d\n",getpid(),getppid(),getpgid(pid));
		exit(0);
	}

	sleep(2);
	printf("i am a child.\n");
	pid_t spid;
	spid = setsid();
	printf("my info: pid = %d,ppid = %d,pgid = %d,spid = %d \n",getpid(),getppid(),getpgid(pid),spid);
	return 0;
}


