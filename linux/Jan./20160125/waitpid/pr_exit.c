#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

void pr_exit(int);

int main(void)
{
	pid_t pid;
	int status;

	if((pid = fork()) < 0)
		perror("fork");

	else if(pid == 0)
		exit(7);

	if(wait(&status) != pid)
		printf("wait error");

	pr_exit(status);

	if((pid = fork()) < 0)
		perror("fork2");
	else if(pid == 0)
		abort();

	if(wait(&status) != pid)
		printf("wait error2");
	pr_exit(status);

	
