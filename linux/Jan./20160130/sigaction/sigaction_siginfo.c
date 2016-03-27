#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include <stdlib.h>

void siga(int signo,siginfo_t *info,void *context)
{
	printf("I get the sig %d\n",signo);
	printf("the send process id is %d\n",info->si_pid);
	printf("the send uid is %d\n",info->si_uid);
}

int main()
{
	struct sigaction act;
	bzero(&act,sizeof(act));
	act.sa_sigaction = siga;
	sigset_t sa_mask;
	sigemptyset(&act.sa_mask);

	act.sa_flags = SA_SIGINFO;
	int ret;
	ret= sigaction(SIGINT,&act,NULL);
	if(-1 == ret){
		perror("sigaction");
		return -1;
	}
	while(1);
	return 0;
}
