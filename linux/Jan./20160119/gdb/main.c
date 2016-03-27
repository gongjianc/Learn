#include <stdio.h>
#include <stdlib.h>

int add(int a,int b)
{
	return a + b;
}

int main(int argc,char *argv[])
{
	if(argc != 3){
		printf("the argc is error\n");
		return 0;
	}
	int a,b;
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	printf("the sum is %d\n",add(a,b));
	return 0;
}
