#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int ticketcount = 20;

void *saleWindow1(void *args)
{
    while(ticketcount > 0){
        printf("window 1 start sale ticket! The ticket is : %d\n", ticketcount);
        sleep(3);
        ticketcount--;
        printf("slae ticket finish! The last ticket is : %d\n", ticketcount);
    }
}
void *saleWindow2(void *args)
{
    while(ticketcount > 0){
        printf("window 2 start sale ticket! The ticket is : %d\n", ticketcount);
        sleep(3);
        ticketcount--;
        printf("slae ticket finish! The last ticket is : %d\n", ticketcount);
    }
}

int main(void)
{
    pthread_t pid1;
    pthread_t pid2;
    pthread_create(&pid1, NULL, saleWindow1, NULL);
    pthread_create(&pid2, NULL, saleWindow2, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    printf("ticket = %d", ticketcount);
    return 0;
}
    
