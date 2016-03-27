#ifndef __CHILD__
#define __CHILD__

#include "cmd.h"
#include "deliver_fd.h"

typedef struct pro_child{
	pid_t pid;
	int fds;
	int busy;
}child,*pchild;


void make_child(pchild,int);

void child_handle(int);
#endif
