#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanup(void* p)
{
	printf("the p is %ld\n",(long)p);
}

void* pthread_func(void* p)
{
	pthread_cleanup_push(cleanup,(void*)1);
	pthread_cleanup_push(cleanup,(void*)2);
	sleep(5);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
}


int main()
{
	pthread_t pthid;
	pthread_create(&pthid,NULL,pthread_func,NULL);
	if(pthread_join(pthid,NULL) != 0){
		printf("error\n");
		return -1;
	}
	return 0;
}
