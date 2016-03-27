#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[])
{
	if(argc != 3){
		printf("error args\n");
		return -1;
	}

	int fdr;

	fdr = open(argv[1],O_RDONLY);
	if(-1 == fdr){
		perror("open1");
		return -1;
	}
	int fdw = open(argv[2],O_WRONLY);
	
	if(-1 == fdw){
		perror("open2");
		return -1;
	}
	
	printf("the fdr = %d,the fdw = %d \n",fdr,fdw);
	char buf[128];
	int ret;
	//ÏÈ¶ÁºóÐ´
	while(bzero(buf,sizeof(buf)),(ret = read(fdr,buf,sizeof(buf))) > 0){

		printf("b: %s\n",buf);
		bzero(buf,sizeof(buf));
		if((ret = read(STDIN_FILENO,buf,sizeof(buf))) > 0){
			write(fdw,buf,ret - 1);
		}else{
			write(fdw,"bye",3);
			break;
			}

	}
	return 0;

}
