#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	if(!fork()){
		printf("I am a child\n");
		exit(0);
	}else{
		while(1);
	}

}
