#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(void)
{
    printf("Parent process id : %d\n", getpid());
    if(fork() == 0){
        printf("I am child, pid is : %d\n", getpid());
        exit(7);
    }else{
        int status;
        sleep(3);
        //status是传出参数，返回exit的值
        pid_t pid = wait(&status);
        printf("I am parent, child id is %d\n", pid);
        if(WIFEXITED(status))
            printf("child exited with code %d\n", WEXITSTATUS(status));
    }



    return 0;
}
