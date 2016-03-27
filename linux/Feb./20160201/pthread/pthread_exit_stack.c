#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

struct foo{
	int a,b,c,d;
};

void printfoo(const char *s,const struct foo *fp)
{
	printf("%s",s);
	printf("structure at 0x%lx\n",(unsigned long)fp);
	printf("foo.a = %d\n",fp->a);
	printf("foo.a = %d\n",fp->b);
	printf("foo.a = %d\n",fp->c);
	printf("foo.a = %d\n",fp->d);
}

void *thr_fn1(void *arg)
{
	struct foo foo = {1,2,3,4};
	printfoo("thread 1:\n",&foo);
	pthread_exit((void *)&foo);
}

void *thr_fn2(void *arg)
{
	printf("thread 2:ID is %lu\n",(unsigned long)pthread_self());
	pthread_exit((void *)0);
}

int main(void)
{
	int ret;
	pthread_t tid1,tid2;
	struct foo *fp;

	ret = pthread_create(&tid1,NULL,thr_fn1,NULL);
	if(0 != ret){
		printf("thread 1\n");
		return -1;
	}

	ret = pthread_join(tid1,(void *)&fp);
	if(0 != ret){
		printf("join 1\n");
		return -1;
	}

	sleep(1);
	printf("parent starting second thread\n");
	ret = pthread_create(&tid2,NULL,thr_fn2,NULL);
	if(0 != ret){
		printf("thread2\n");
		return -1;
	}
	sleep(1);
	printfoo("parent:\n",fp);
	exit(0);
}
