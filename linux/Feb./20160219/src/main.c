#include "factory.h"

int main(int argc,char **argv)
{
	if(argc != 5)
	{
		printf("error args\n");
		return -1;
	}

	int thread_num = atoi(argv[3]);
	int capacity = atoi(argv[4]);//队列里边能创建多少描述符
	factory tf;
	factory_init(&tf,thread_num,capacity,thread_handle);
	//printf("tags 1\n");
	factory_start(&tf);//产生线程
	//printf("tags 2\n");
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[2]));
	ser.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	ret = bind(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	if(-1 == ret){
		perror("bind");
		return -1;
	}
	listen(sfd,capacity);
	int epfd = epoll_create(1);
	struct epoll_event ev,evs[2];
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);
	if(-1 == ret){
		perror("epoll_ctl");
		return -1;
	}
	int new_fd;
	while(1){
		ret = epoll_wait(epfd,evs,2,-1);
		if(ret > 0){
			if(evs[0].data.fd == sfd && evs[0].events == EPOLLIN){
				new_fd = accept(sfd,NULL,NULL);
				pnode pnew;
				pnew = (pnode)calloc(1,sizeof(node));//使用指针要先分配空间
				pnew->new_fd = new_fd;
				factory_que_set(&tf.fd_que,pnew);
				pthread_cond_signal(&tf.cond);
			}
		}
	}
}

void* thread_handle(void* arg)
{
	pfactory pf = (pfactory)arg;
	printf("the start_flag is %d\n",pf->start_flag);
	pque_t pq = &pf->fd_que;
	pnode pcur;
	if(factory_que_empty(pq)){
		pthread_cond_wait(&pf->cond,&pq->mutex);
		pthread_mutex_unlock(&pq->mutex);
	}
	factory_que_get(pq,&pcur);
	send_file(pcur->new_fd);
	free(pcur);
}

void send_file(int sfd)
{
	int ret;
	data_t buf;
	buf.len = strlen(DOWN_FILE);
	strcpy(buf.buf,DOWN_FILE);
	ret = send(sfd,&buf,buf.len + 4,0);//发送文件名
	if(-1 == ret){
		perror("send1");
		return;
	}

	int fd;
	fd = open(DOWN_FILE,O_RDONLY);
	if(-1 == fd){
		perror("open");
		return;
	}
	while(bzero(&buf,sizeof(buf)),(buf.len = read(fd,buf.buf,sizeof(buf.buf))) > 0){
		send_n(sfd,(char*)&buf,buf.len + 4);//4data_t结构体中的len的长度
	}

	//结束标志
	int flag;
	bzero(&buf,sizeof(buf));
	buf.len = sizeof(int);
	memcpy(buf.buf,&flag,4);
	send(sfd,&buf,buf.len + 4,0);
	close(sfd);
	return;
}
