#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int ticketcount = 10;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *salewindow(void *args)
{
    while(1){
        pthread_mutex_lock(&mutex);
        if(ticketcount > 0){
            printf("window %d start sale ticket! The ticket is : %d\n", (int)args, ticketcount);
            ticketcount--;
            if(ticketcount == 0)
                pthread_cond_signal(&cond);
            printf("sale ticket finish! The last ticket is %d\n", ticketcount);
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void *setticket(void *args)
{
    pthread_mutex_lock(&mutex);
    if(ticketcount > 0)
        pthread_cond_wait(&cond, &mutex);
    ticketcount = 10;
    pthread_mutex_unlock(&mutex);
    sleep(1);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t pid1, pid2, pid3;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&pid1, NULL, salewindow, (void *)1);
    pthread_create(&pid2, NULL, salewindow, (void *)2);
    pthread_create(&pid3, NULL, setticket, NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    pthread_join(pid3, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

