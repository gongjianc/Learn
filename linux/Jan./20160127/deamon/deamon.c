#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(void)
{
	umask(0);

	if(fork() != 0)
		exit(0);

	setsid();
	chdir("/");
	int i;
	for(i = 0;i < 3;i++)
		close(i);

	while(1);
	return 0;
}

