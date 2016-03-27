#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    printf("uid : %d, gid : %d\n",getuid(), getgid());
    printf("euid : %d, egid : %d\n",geteuid(), getegid());

    FILE *fp = fopen("a.txt", "a");

    if(fp == NULL){
        perror("fopen error");
        exit(-1);
    }

    fputs("world", fp);
    fclose(fp);
    return 0;
}
