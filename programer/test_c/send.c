#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MAX_TEXT 2048

struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};

int main()
{
    int running = 1;
    int msgid = -1;
    struct msg_st data;
    char buffer[BUFSIZ];

    //建立消息队列
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if(msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    //从队列中获取消息，直到遇到end消息为止
    while(running)
    {
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        data.msg_type = 1;
        strcpy(data.text, buffer);
        if(msgsnd(msgid, (void *)&data, MAX_TEXT, 0) == -1){
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        if(strncmp(buffer, "end", 3) == 0)
            running = 0;
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}
