#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

static void
sigHandler(int sig)
{
    UNUSED(sig);
    printf("Ouch\n");
}

int main(void)
{
    int j;

    if(signal(SIGINT, sigHandler) == SIG_ERR){
        printf("signal\n");
        return -1;
    }

    for(j = 0; ; j++){
        printf("%d\n", j);
        sleep(3);
    }
    return 0;
}
