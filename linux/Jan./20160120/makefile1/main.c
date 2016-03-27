#include <stdio.h>
#include <stdlib.h>

int mul(int,int);
int main(int argc,char *argv[])
{
	if(argc != 3){
		printf("error args\n");
		return 0;
	}

	int a,b;
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	printf("the a*b is %d \n",mul(a,b));
	return 0;
}
