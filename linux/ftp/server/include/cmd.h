#ifndef __CMD__
#define __CMD__

#include "head.h"
#include "pool_n.h"

typedef struct tdata{
	int len;
	char buf[1020];
}data_t,*pdata_t;

void cmd_parser(char*,int);

void cmd_pwd(char**);

void cmd_cd(char**,char*);

void cmd_ls(char**);

void cmd_error(char**);

void cmd_gets(char**,char*,int);

void cmd_puts(char**,char*,int);

void cmd_remove(char**,char*);

void send_file(char*,int);

void recv_file(char*,int);

#endif
