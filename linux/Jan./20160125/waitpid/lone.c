#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	if(!fork()){
		printf("I am a child\n");
		while(1);
	}else{
		exit(0);
	}
}
