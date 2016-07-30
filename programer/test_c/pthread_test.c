#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

typedef void *(*fun)(void *);
int g_Flag = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread1(void *);
void *thread2(void *);
int main(void)
{
    printf("enter main\n");
    pthread_t tid1, tid2;
    int res1 = 0, res2 = 0;
    res2 = pthread_create(&tid2, NULL, thread2, NULL);
    if(res2 != 0)
        printf("%s: %d\n", __func__, strerror(res2));
    res1 = pthread_create(&tid1, NULL, thread1, NULL);
    if(res1 != 0)
        printf("%s: %d\n", __func__, strerror(res1));

    return 0;
}
