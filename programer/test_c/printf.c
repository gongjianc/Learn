#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char buf[40];
    setbuf(stdout, buf);
    while(1){
        fprintf(stdout, "hello-std-out");
        fprintf(stderr, "hello-std-err");
        sleep(1);
    }
    return 0;
}
