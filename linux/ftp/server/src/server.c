#include "child.h"

int main(int argc,char **argv)
{
	if(argc != 4){
		printf("error args\n");
		return -1;
	}
	int num = atoi(argv[3]);
	//�����ӽ��̣�ҵ�����
	pchild cptr;
	cptr = (pchild)calloc(num,sizeof(child));
	make_child(cptr,num);
	//����socket����
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}
	//��ֹ�쳣�رգ��޷��򿪶˿�
	int on = 1;
	int ret;
	ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(-1 == ret){
		perror("setsockopt");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[2]));
	ser.sin_addr.s_addr  = inet_addr(argv[1]);
	ret = bind(sfd,(struct sockaddr *)&ser,sizeof(struct sockaddr));
	if(-1 == ret){
		perror("bind");
		return -1;
	}
	listen(sfd,num);
	//epoll����ע��
	int epfd;
	epfd = epoll_create(1);
	if(-1 == epfd){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev,*evs;
	evs = (struct epoll_event*)calloc(num + 1,sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = sfd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);
	if(-1 == ret){
		perror("epoll_ctl");
		return -1;
	}
	int i;
	for(i = 0;i < num;i++){
		evs[i].events = EPOLLIN;
		evs[i].data.fd = cptr[i].fds;
		ret = epoll_ctl(epfd,EPOLL_CTL_ADD,cptr[i].fds,&evs[i]);
		if(-1 == ret){
			perror("epoll_ctl");
			return -1;
		}
	}

	char buf[128];
	//epoll_wait������æ���ӽ��̣����ͻ����������ɵ�new_fd���͸��ӽ���
	int j;
	int new_fd;
	while(1){
		ret = epoll_wait(epfd,evs,num + 1,-1);
		if(ret > 0){
			for(i = 0;i < ret;i++){
				//������sfdʱ��Ĳ���
				if(evs[i].data.fd == sfd && evs[i].events == EPOLLIN){
					new_fd = accept(sfd,NULL,NULL);
					printf("one client connect\n");	
					for(j = 0;j < num;j++){
						if(cptr[j].busy == 0){
							break;
						}
					}
					
					if(j != num){
						send_fd(cptr[j].fds,new_fd);/*fds��sendmsgʹ�õ�*/
						//��new_fd����Ӧ�ӽ��̺��ʶΪæµ
						cptr[j].busy = 1;
					}
				}
				for(j = 0;j < num;j++){
					if(evs[i].data.fd == cptr[j].fds && evs[i].events == EPOLLIN){
						//���յ��ӽ��̵�����1�Ժ󣬽��ӽ��̱�Ϊ����
						cptr[j].busy = 0;
					}
				}
			}
		}
	}
	//close(new_fd);
	close(sfd);
}

