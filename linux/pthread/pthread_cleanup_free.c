#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void freemem(void *args)
{
    free(args);
    printf("clean up the memery!\n");
}

void *threadFunc(void *args)
{
    char *p = (char *)malloc(10);
    pthread_cleanup_push(freemem, p);
    for(int i = 0; i < 10; i++){
        printf("hello, my name is xiaoming!\n");
        sleep(1);
    }
    pthread_exit((void *)3);
    pthread_cleanup_pop(0);
}

int main(void)
{
    pthread_t pid;
    pthread_create(&pid, NULL, threadFunc, NULL);
    for(int i = 1; i < 5; i++){
        printf("hello, nice to meet you!\n");
        sleep(1);
        if(i % 3 == 0)
            pthread_cancel(pid);
    }

    int ret = 0;
    pthread_join(pid, (void **)&ret);
    printf("return value is :%d\n", ret);
    return 0;
}

