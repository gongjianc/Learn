#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	if(argc != 3){
		printf("error args\n");
		return -1;
	}
	int fdw;
	fdw = open(argv[1],O_WRONLY);
	if(-1 == fdw){
		perror("open 1");
		return -1;
	}
	int fdr;
	fdr = open(argv[2],O_RDONLY);
	if(-1 == fdr){
		perror("open2");
		return -1;
	}
	printf("the fdw = %d,the fdr = %d \n",fdw,fdr);
	char buf[128];
	int ret;
	//ÏÈÐ´ºó¶Á
	while(bzero(buf,sizeof(buf)),(ret = read(STDIN_FILENO,buf,sizeof(buf))) > 0 ){
		write(fdw,buf,ret - 1);

		bzero(buf,sizeof(buf));
		if((ret = read(fdr,buf,sizeof(buf))) > 0){
			printf("a: %s\n",buf);
			if(!strcmp(buf,"bye"))
					break;
		}
	}

	return 0;

}
