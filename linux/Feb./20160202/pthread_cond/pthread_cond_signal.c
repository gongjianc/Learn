#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

void *p_func(void *p)
{
	pthread_mutex_lock(&mutex);
	int ret;
	printf("I am child ,I am here\n");
	ret = pthread_cond_wait(&cond,&mutex);
	if(0 != ret){
		printf("pthread_cond_wait ret = %d\n",ret);
	}
	printf("I am child , I am wake\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main()
{
	int ret;
	ret = pthread_cond_init(&cond,NULL);
	if(0 != ret){
		printf("pthread_cond_init ret = %d\n",ret);
		return -1;
	}
	ret = pthread_mutex_init(&mutex,NULL);
	if(0 != ret){
		printf("pthread_mutex_init ret = %d\n",ret);
		return -1;
	}
	pthread_t thid;
	pthread_create(&thid,NULL,p_func,NULL);
	sleep(1);
	pthread_mutex_lock(&mutex);
	printf("I am main thread,I can lock\n");
	ret = pthread_join(thid,NULL);
	if(0 != ret){
		printf("pthread_join ret = %d\n",ret);
		return -1;
	}

	ret = pthread_cond_destroy(&cond);
	if(0 != ret){
		printf("pthread_cond_destroy ret = %d\n",ret);
		return -1;
	}

	pthread_mutex_destroy(&mutex);
	return 0;
}
	

