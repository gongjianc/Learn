#include <stdio.h>
#include <string.h>

int main(int argc,char* argv[])
{
	int a;
	int ret;
	char c[30];
	strcpy(c,"123");
	
	ret = sscanf(c,"%d",&a);
	printf("the a is %d\n",a);
	printf("the ret is %d\n",ret);
	
	memset(c,0,sizeof(c));
	sprintf(c,"%s%s",argv[1],argv[0] + 1);
	printf("%s\n",c);
	return 0;
}
