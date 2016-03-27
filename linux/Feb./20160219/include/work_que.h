#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "head.h"


typedef struct node_fd{
	int new_fd;
	struct node_fd* pnode;
}node,*pnode;

typedef struct que_fd{
	pnode que_phead,que_ptail;
	pthread_mutex_t mutex;
	int size; //当前放置的描述符个数
	int que_capacity;//队列最大长度
}que_t,*pque_t;

void factory_que_init(pque_t,int);

void factory_que_set(pque_t,pnode);

void factory_que_get(pque_t,pnode*);

int factory_que_empty(pque_t);
#endif
