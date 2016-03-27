#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	if(!fork()){
		printf("I am a child,and my pid is:%d\n",getpid());
		sleep(3);
		exit(0);
	}else{
		int status;
		int ret;
		ret = wait(&status);
		printf("i am someone`s parent\n");
		if(WIFEXITED(status)){
			printf("the exit value is %d\n",WEXITSTATUS(status));
			printf("the pid of which child has terminated is :%d \n",ret);
		}
		return 0;
	}

}
