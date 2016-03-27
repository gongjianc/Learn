#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

void send_fd(int sfd,int fd)
{
	struct msghdr msg;
	bzero(&msg,sizeof(struct msghdr));
	char buf1[] = "hello";
	char buf2[] = "world";
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;

	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	int *fdptr;
	fdptr = (int*)CMSG_DATA(cmsg);
	*fdptr = fd;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = sendmsg(sfd,&msg,0);
	if(-1 == ret){
		perror("sendmsg");
		return;
	}
}

void recv_fd(int sfd,int *fd)
{
	struct msghdr msg;
	bzero(&msg,sizeof(struct msghdr));
	char buf1[10] = {0};
	char buf2[10] = {0};
	struct iovec iov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 5;
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;

	struct cmsghdr *cmsg;
	int len = CMSG_LEN(sizeof(int));
	cmsg = (struct cmsghdr *)calloc(1,len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;
	int ret = recvmsg(sfd,&msg,0);
	if(-1 == ret){
		perror("sendmsg");
		return;
	}
	*fd = *(int*)CMSG_DATA(cmsg);
	//printf("the buf1 is %s",buf1);
}

int main(int argc,char **argv)
{
	if(argc != 2){
		printf("error args\n");
		return -1;
	}

	int fds[2];
	int ret;
	ret = socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
	if(-1 == ret){
		perror("socketpair");
		return -1;
	}

	if(!fork()){
		close(fds[1]);
		int fd;
		recv_fd(fds[0],&fd);
		printf("I am child,fd is %d\n",fd);
		char buf[10] = {0};
		read(fd,buf,sizeof(buf));
		printf("the buf is %s\n",buf);
		exit(0);
	}else{
		close(fds[0]);
		int fd = open(argv[1],O_RDWR);
		printf("I am parent,fd is %d\n",fd);
		send_fd(fds[1],fd);
		wait(NULL);
		return 0;
	}

}
