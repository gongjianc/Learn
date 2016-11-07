#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PERM S_IRUSR|S_IWUSR

int main(void)
{
    int shmid = shmget(IPC_PRIVATE, 1024, PERM);
    if(-1 == shmid){
        perror("create shared memory");
        exit(-1);
    }

    if(fork() > 0){
        char *p_addr = (char *)shmat(shmid, NULL, 0);
        memset(p_addr, '\0', 1024);
        strncpy(p_addr, "share memory", 1024);
        printf("parent pid = %d, write buffer is : %s\n", getpid(), p_addr);
        sleep(2);
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    }else{
        sleep(5);
        char *c_addr = (char *)shmat(shmid, NULL, 0);
        printf("client pid = %d, shmid = %d, read buffer is : %s\n", getpid(), shmid, c_addr);
        exit(0);
    }
}
