#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define DOWN_FILE "file"

typedef struct pro_child{
	pid_t pid;
	int fds;//socket的一端，既可读，又可写
	int busy;
}child,*pchild;

typedef struct tdata{
	int len;
	char buf[1020];
}data_t,*pdata_t;

void make_child(pchild,int);

void child_handle(int);

void send_file(int);

void send_fd(int,int);

void recv_fd(int,int*);

void send_n(int,char*,int);

void recv_n(int,char*,int);
