#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
	DIR *dir;
	if(argc != 2){
		printf("args error\n");
		return -1;
	}else
		dir = opendir(argv[1]);

	struct dirent sdir;
	
