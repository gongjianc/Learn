#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *threadFunc(void *args)
{
    char *p = (char *)malloc(10);
    for(int i = 0; i < 10; i++){
        printf("hello, my name is wangxiao!\n");
        sleep(1);
    }
    free(p);
    printf("p is freed\n");
    pthread_exit((void *)3);
}

int main(void)
{
    pthread_t pid;
    pthread_create(&pid, NULL, threadFunc, NULL); 
    for(int i = 1; i< 5; i++){
        printf("hello, nice to meet you!\n");
        sleep(1);
        if(i % 3 == 0)
           pthread_cancel(pid);
    }
    int ret = 0;
    pthread_join(pid, (void **)&ret);
    printf("return value is : %d\n", ret);
    return 0;
}
