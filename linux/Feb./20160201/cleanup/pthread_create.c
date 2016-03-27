#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *thr_fn1(void *arg)
{
	printf("thread 1 returning\n");
	return((void*)1);
}

void *thr_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	return((void*)2);
}

int main(void)
{
	int ret;
	pthread_t tid1,tid2;
	void *tret;

	ret = pthread_create(&tid1,NULL,thr_fn1,NULL);
	if(ret != 0){
		printf("can`t create thread 1");
		return -1;
	}
	
	ret = pthread_create(&tid2,NULL,thr_fn2,NULL);
	if(ret != 0){
		printf("can`t create thread 2");
		return -1;
	}

	ret = pthread_join(tid1,&tret);
	if(ret != 0){
		printf("can`t join thread 1");
		return -1;
	}
	printf("thread 1 exit code %ld\n",(long)tret);

	ret = pthread_join(tid2,&tret);
	if(ret != 0){
		printf("can`t join thread 2");
		return -1;
	}
	printf("thread 2 exit code %ld\n",(long)tret);

	exit(0);
}
