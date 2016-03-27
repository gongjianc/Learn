#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	if(argc != 2){
		printf("args error\n");
		return -1;
	}

	int ret;
	ret = rmdir(argv[1]);
	if(-1 == ret)
		printf("mkdir error\n");
	else
		printf("the ret is %d",ret);
}
