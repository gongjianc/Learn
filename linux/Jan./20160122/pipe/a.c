#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

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
	fd_set readset;


	//ÏÈ¶ÁºóÐ´
	while(1){
		FD_ZERO(&readset);
		FD_SET(STDIN_FILENO,&readset);
		FD_SET(fdr,&readset);
		ret = select(fdr + 1,&readset,NULL,NULL,NULL);
		if(ret > 0){	
			if(FD_ISSET(STDIN_FILENO,&readset)){
				if((ret = read(STDIN_FILENO,buf,sizeof(buf))) > 0 ){
					if(!strcmp(buf,"bye")){
						write(fdw,buf,ret - 1);

						break;
					}
					write(fdw,buf,ret - 1);
				}

			}
			if(FD_ISSET(fdr,&readset)){
				bzero(buf,sizeof(buf));
				if((ret = read(fdr,buf,sizeof(buf))) > 0){
					printf("b:%s\n",buf);
				}	
			}
		}
	}
	close(fdr);
	close(fdw);
	return 0;
}
