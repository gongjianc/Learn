#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig(int signal)
{
	printf("the signal is %d\n",signal);
	sleep(5);
	printf("after sleep,the signal is %d\n",signal);
}

int main()
{
	signal(SIGINT,sig);
	signal(SIGQUIT,sig);
	while(1);
	return 0;
}
