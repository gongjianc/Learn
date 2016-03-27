#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanFunc(void *args)
{
    printf("cleanFunc(%d)\n", (int)args);
}

void *threadFunc(void *args)
{
    pthread_cleanup_push(cleanFunc, (void *)1);
    pthread_cleanup_push(cleanFunc, (void *)2);
    sleep(2);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}

int main(void)
{
    pthread_t pid;
    pthread_create(&pid, NULL, threadFunc, (void *)2);
    pthread_join(pid, NULL);
    return 0;
}
