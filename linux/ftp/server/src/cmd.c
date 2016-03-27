#include "cmd.h"

void cmd_parser(char* buf,int sfd)
{
	char *p = (char*)calloc(1,sizeof(char));
	p = buf;//Ϊʲô����*p = *buf;
	char cmd[10];
	char index[128];
	bzero(cmd,sizeof(cmd));
	bzero(index,sizeof(index));

	int i = 0;
	int j = 0;
	while(*p != ' ' &&  *p != '\n'){
		cmd[i] = *p;
		p++;
		i++;
	}
	if(*p != '\n'){
		p++;
		while(*p != '\n'){
			index[j] = *p;
			p++;
			j++;
		}
	}

	if(!memcmp(cmd,"pwd",3)){
		cmd_pwd(&buf);	
	}else if(!memcmp(cmd,"cd",2)){
		cmd_cd(&buf,index);
	}else if(!memcmp(cmd,"ls",2)){
		cmd_ls(&buf);
	}else if(!memcmp(cmd,"puts",4)){
		cmd_puts(&buf,index,sfd);
	}else if(!memcmp(cmd,"gets",4)){
		cmd_gets(&buf,index,sfd);
	}else if(!memcmp(cmd,"remove",6)){
		cmd_remove(&buf,index);
	}else{
		cmd_error(&buf);

	}
}

void cmd_pwd(char** buf)
{
	printf("cmd_pwd\n");
	bzero(*buf,sizeof(*buf));
	char buf1[128];
	bzero(buf1,sizeof(buf1));
	getcwd(buf1,sizeof(buf1));
	//printf("buf1 is %s\n",buf1);
	memcpy(*buf,buf1,sizeof(buf1));
	//printf("getcwd is %s\n",*buf);
}
 
void cmd_cd(char** buf,char* index)
{
	int ret;
	ret = chdir(index);
	if(-1 == ret){
		perror("chdir");
		return;
	}
	printf("cmd_cd\n");
	bzero(*buf,sizeof(*buf));
	char buf1[128];
	bzero(buf1,sizeof(buf1));
	getcwd(buf1,sizeof(buf1));
	//printf("buf1 is %s\n",buf1);
	memcpy(*buf,buf1,sizeof(buf1));
}
	
void cmd_ls(char **buf)
{
	printf("cmd_ls\n");
	DIR *pDir;
	pDir = opendir(".");
	struct dirent *pDirInfo;
	bzero(*buf,sizeof(*buf));
	while((pDirInfo = readdir(pDir)) != NULL){
		if(memcmp(pDirInfo->d_name,".",1) != 0 && memcmp(pDirInfo->d_name,"..",2) != 0){
			strcat(*buf,pDirInfo->d_name);
			strcat(*buf," ");
		}		
	}
	closedir(pDir);

}

void cmd_gets(char** buf,char* index,int sfd)
{
	printf("cmd_gets\n");
	//printf("index is %s,len is %d\n",index,(int)strlen(index));
	DIR *pDir;
	pDir = opendir(".");
	struct dirent *pDirInfo;
	while((pDirInfo = readdir(pDir)) != NULL){
		if(memcmp(pDirInfo->d_name,index,strlen(index)) == 0){
			bzero(*buf,strlen(*buf));
			memcpy(*buf,"downloading...",14);
			send(sfd,*buf,strlen(*buf),0);	
			printf("gets send\n");
			printf("start send file,buf is %s\n",*buf);	

			send_file(index,sfd);

			bzero(*buf,strlen(*buf));
			memcpy(*buf,"download finish.",15);
			printf("after send file,buf is %s\n",*buf);
			closedir(pDir);
			return;
		}
	}
	memcpy(*buf,"wrong file name!",16);
	closedir(pDir);
}

void cmd_puts(char** buf,char* index,int sfd)
{
	printf("cmd_puts\n");
	//chdir("../upload");
	bzero(*buf,strlen(*buf));
	memcpy(*buf,"uploading...",12);
	send(sfd,*buf,strlen(*buf),0);	
	send(sfd,index,strlen(index),0);
	recv_file(index,sfd);
	bzero(*buf,strlen(*buf));
	memcpy(*buf,"upload success",14);
	send(sfd,*buf,strlen(*buf),0);	
}

void cmd_remove(char** buf,char* index)
{
	printf("cmd_remove\n");
	DIR *pDir;
	pDir = opendir(".");
	struct dirent *pDirInfo;
	int ret;
	while((pDirInfo = readdir(pDir)) != NULL){
		if(memcmp(pDirInfo->d_name,index,strlen(index)) == 0){
			ret = remove(index);
			if(-1 == ret){
				perror("remove");
				return;
			}
			bzero(*buf,strlen(*buf));
			memcpy(*buf,"remove success",14);
			closedir(pDir);
			return;
		}
	}
	memcpy(*buf,"wrong file name!",16);

}

void cmd_error(char **buf)
{
	bzero(*buf,sizeof(*buf));
	memcpy(*buf,"CMD ERROR!",10);
}
void send_file(char* index,int sfd)
{
	int ret;
	int flag = 0;
	data_t data;

	data.len = strlen(index);
	strcpy(data.buf,index);
	printf("data.len is %d\n",data.len);
	printf("data.buf is %s\n",data.buf);

	/*data.len��data.buf�ĳ��ȣ�+4���ټ���int len��4���ֽڵĳ���*/
	ret = send(sfd,&data,data.len + 4,0);//?�����ļ���
	if(-1 == ret){
		perror("send1");
		return;
	}
	int fd = open(index,O_RDONLY);
	if(-1 == fd){
		perror("open");
		return;
	}
	//�˴���������취��ֱ�ӽ�data.len����Ϊread�ķ���ֵ
	while(bzero(&data,sizeof(data)),(data.len = read(fd,data.buf,sizeof(data.buf))) > 0){
		send_n(sfd,(char*)&data,data.len + 4);
	
		//send(sfd,&data,data.len + 4,0);
	}

	data.len = sizeof(int);
	memcpy(data.buf,&flag,4);
	ret = send(sfd,&data,data.len + 4,0);//?���ͽ�����
	if(-1 == ret){
		perror("send2");
		return;
	}
	printf("send file success\n");
//	close(sfd);
	return;

}

void recv_file(char* index,int sfd)
{
	printf("ready to receive files\n");
	data_t data;
	bzero(&data,sizeof(data));
	//�����ļ���
	int ret;
	ret = recv(sfd,&data.len,sizeof(int),0);
	if(-1 == ret){
		perror("recv1");
		return;
	}
	recv(sfd,data.buf,data.len,0);
	int fd;
	fd = open(data.buf,O_RDWR | O_CREAT,0666);
	if(-1 == fd){
		perror("open");
		return;
	}
	int flag = 0;
	while(1){
		bzero(&data,sizeof(data));
		ret= recv(sfd,&data.len,4,0);
		if(-1 == ret){
			perror("recv2");
			return;
		}
		//Ϊʲô���ﲻ��recv_n
		recv(sfd,data.buf,data.len,0);
		if(-1 == ret){
			perror("recv3");
			return;
		}
		//�����ļ�����
		if(data.len == sizeof(int) && !memcmp(data.buf,&flag,4)){
			break;
		}
		ret = write(fd,data.buf,data.len);
		if(ret < 0){
			perror("write");
			return;
		}
	}
}
