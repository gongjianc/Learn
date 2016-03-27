#include "work_que.h"

void factory_que_init(pque_t pq,int capacity)
{
	pq->que_phead = NULL;
	pq->que_ptail = NULL;
	int ret;
	ret = pthread_mutex_init(&pq->mutex,NULL);
	if(0 != ret){
		printf("pthread_mutex_init fail\n");
		return;
	}
	pq->size = 0;
	pq->que_capacity = capacity;
}

//尾插法建立链表
void factory_que_set(pque_t pq,pnode pnew)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->que_ptail == NULL){
		pq->que_phead = pnew;
		pq->que_ptail = pnew;
	}else{
		(pq->que_ptail)->pnode = pnew;
		pq->que_ptail = pnew;
	}
	pq->size++;
	pthread_mutex_unlock(&pq->mutex);
}

void factory_que_get(pque_t pq,pnode* pcur)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->que_phead != NULL){
		*pcur = pq->que_phead;
		pq->que_phead = (pq->que_phead)->pnode;
		pq->size--;
	}
	pthread_mutex_unlock(&pq->mutex);
}

int factory_que_empty(pque_t pq)
{
	pthread_mutex_lock(&pq->mutex);
	if(pq->size == 0){
		pthread_mutex_unlock(&pq->mutex);
		return 1;
	}
	pthread_mutex_unlock(&pq->mutex);
	return 0;
}
