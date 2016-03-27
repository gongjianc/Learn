#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *threadFunc(void *pArg)
{
    for(int i = 0; i < 10; i++){
        printf("Hi, I'm child thread, arg is : %d\n", (int)pArg);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t pid;
    pthread_create(&pid, NULL, threadFunc, (void *)123);
    for(int i = 0; i< 10; i++){
        printf("Hi, I'm main thread, child thread id is : %x\n", pid);
        sleep(1);
    }
    return 0;
}
