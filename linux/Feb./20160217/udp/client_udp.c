#include "func.h"

int main(int argc,char** argv)
{
	int sfd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == sfd){
		perror("socket");
		return -1;
	}

	struct sockaddr_in server;
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;

	char buf[128];
	struct sockaddr_in client;
	bzero(buf,sizeof(buf));
	bzero(&client,sizeof(client));
	int clen = sizeof(struct sockaddr);

	ret = sendto(sfd,"I am client",11,0,(struct sockaddr *)&server,sizeof(struct sockaddr));
	if(-1 == ret){
		perror("sendto");
		return -1;
	}
	ret = recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr *)&client,&clen);
	if(-1 == ret){
		perror("recvfrom");
		return -1;
	}
	printf("I am client I recvform %s\n",buf);
	printf("IP = %s,PORT = %d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));


	close(sfd);
	return 0;
}
	
