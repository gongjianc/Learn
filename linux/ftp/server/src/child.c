#include "child.h"

void make_child(pchild cptr,int n)
{
	int i;
	int fds[2];
	int ret;
	int pid;
	for(i = 0;i < n;i++){
		ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);	
		if(-1 == ret){
			perror("socketpair");
			return;
		}
		pid = fork();
		//¸¸½ø³Ì
		if(pid == 0){
			close(fds[1]);
			child_handle(fds[0]);
		}
		close(fds[0]);
		cptr[i].pid = pid;
		cptr[i].fds = fds[1];
		cptr[i].busy = 0;
	}
}	

void child_handle(int fdr)
{
	int sfd;
	recv_fd(fdr,&sfd);
	char buf[128];
	int ret;
	while(1){
		bzero(buf,sizeof(buf));
		ret = recv(sfd,buf,sizeof(buf),0);
		if(-1 == ret){
			perror("accept");
			return;
		}
		printf("recv:buf is %s",buf);
		cmd_parser(buf,sfd);
		printf("After handle:buf is %s\n",buf);
		send(sfd,buf,strlen(buf),0);

	}
}

