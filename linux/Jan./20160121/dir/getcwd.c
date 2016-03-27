#include <stdio.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	char buf[30];	
	getcwd(buf,sizeof(buf));
	printf("the path is %s \n",buf);
	return 0;
}
