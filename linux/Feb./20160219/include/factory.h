#ifndef __FACTORY__
#define __FACTORY__
#include "head.h"
#include "work_que.h"

#define DOWN_FILE "hello.avi"
typedef void* (*pfunc)(void*);

typedef struct thread_pool{
	pthread_cond_t cond; //控制线程同步的条件变量
	pthread_t* pth;//不确定有多少线程，所以用指针
	pfunc entry;//线程入口函数,多个不同类型的线程，可以设置多个入口函数
	que_t fd_que;
	int start_flag;//线程是否启动的标志
	int thread_num;//线程数	
}factory,*pfactory;

typedef struct tdata{
	int len;
	char buf[1020];
}data_t,*pdata_t;

void factory_init(pfactory,int,int,pfunc);

void factory_start(pfactory); //创建线程

void* thread_handle(void*);

void send_file(int);

void send_n(int,char*,int);



#endif
