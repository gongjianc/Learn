#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>

typedef struct tdata{
	int len;
	char buf[1020];
}data_t,*pdata_t;

void send_n(int,char*,int);

void recv_n(int,char*,int);
