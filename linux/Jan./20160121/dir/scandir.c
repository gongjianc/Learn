#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void myscandir(char *path,int width)
{
	DIR *dir;
	dir = opendir(path);
	if(NULL == dir)
	{
		perror("opendir");
	}
	struct dirent *pt;
	char buf[128];
	while(pt = readdir(dir))
	{
		if(!strcmp(pt->d_name,".") || !strcmp(pt->d_name,".."))
		{
		}else{
			printf("%*s%s\n",width," ",pt->d_name);
			if(pt->d_type != 4)
			{
			}else{
				      bzero(buf,sizeof(buf));
				      sprintf(buf,"%s%s%s",path,"/",pt->d_name);
				      myscandir(buf,width + 4);
			      }
		
		}
	}
	closedir(dir);
	
}




int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		printf("error args\n");
		return -1;
	}
	printf("%s\n",argv[1]);
	myscandir(argv[1],0);

}
