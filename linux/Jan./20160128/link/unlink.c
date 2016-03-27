#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	if(argc != 2){
		printf("args error\n");
		return -1;
	}

	if(open(argv[1],O_RDWR) < 0){
		perror("open");
		return -1;
	}

	if(unlink(argv[1]) < 0){
		perror("unlink");
		return -1;
	}

	printf("file unlinked\n");
	sleep(15);

	printf("done\n");
	exit(0);
}

