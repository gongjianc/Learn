#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *threadFunc(void *pArg)
{
    int i = 0;
    for(i; i < 10; i++){
        printf("Hi, I`m child thread, arg is :%d\n", (int)pArg);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t thdId;
    pthread_create(&thdId, NULL, threadFunc, (void *)123);
    int i = 0;
    for(i; i < 10; i++){
        printf("Hi, i`m main thread, child thread id is: %x\n", thdId);
        sleep(1);
    }
    return 0;
}
