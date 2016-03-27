#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanup(void *arg)
{
	printf("cleanup:the arg is %ld\n",(long)arg);
}

void *pthread_func(void *arg)
{
	pthread_cleanup_push(cleanup,(void *)1);
	pthread_cleanup_push(cleanup,(void *)2);
	sleep(5);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
}

int main(void)
{
	void *tret;
	pthread_t pthid;
	pthread_create(&pthid,NULL,pthread_func,NULL);
	int ret;
	ret = pthread_cancel(pthid);
	if(0 != ret){
		printf("pthread_cancel :ret = %d\n",ret);
		return -1;
	}
	ret = pthread_join(pthid,&tret);
	if(0 != ret){
		printf("pthread_join:ret = %d\n",ret);
		return -1;
	}
	printf("thread exit code %ld\n",(long)tret);
	return 0;
}

