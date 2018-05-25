/*
** @file    server.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-24 23:42:47
*/
 
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
 
using std::cout;
using std::endl;

int main(void)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listenfd){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr));
    if(-1 == ret){
        perror("bind error");
        close(listenfd);
        exit(EXIT_FAILURE);
    }
    
    ret = listen(listenfd, 10);
    if(-1 == ret){
        perror("listen error");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    int connectfd = accept(listenfd, NULL, NULL);
    char buf[1024] = "welcome to server.";
    ret = write(connectfd, buf, strlen(buf));

    while(1){
        bzero(buf, 0);
        ret = read(connectfd, buf, 1024);
        write(connectfd, buf, strlen(buf));
    }

    return 0;
}
