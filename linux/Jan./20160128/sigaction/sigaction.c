#include <stdio.h>
#include <signal.h>
#include <strings.h>


void siga(int signum,siginfo_t *p,void *q)
{
	printf("the signal is %d\n",signum);
}

int main()
{
	struct sigaction act;
	bzero(&act,sizeof(act));
	
	act.sa_sigaction = siga;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;


	int ret;
	ret = sigaction(SIGINT,&act,NULL);

	while(1);
	return 0;
}
