#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

#define PROJ_ID 1

int qflag = 0;

void sig(int signum)
{
 	qflag = 1;
	printf("\nIt is terminating.\n");
}
int main(int argc,char* argv[])
{
	signal(SIGINT,sig);
	if(argc !=4)
	{
		printf("error args\n");
		return -1;
	}
	int fdw;
	fdw=open(argv[1],O_WRONLY);
	if(-1==fdw)
	{
		perror("open2");
		return -1;
	}
	int fdr;
	fdr=open(argv[2],O_RDONLY);
	if(-1==fdr)
	{
		perror("open1");
		return -1;
	}
//	printf("fdr=%d,fdw=%d\n",fdr,fdw);
	printf("welcome\n");

	//产生key
	key_t skey;
	skey = ftok(argv[3],PROJ_ID);
	if(-1 == skey){
		perror("ftok");
		return -1;
	}
	//共享内存	
	int shmid;
	shmid = shmget(skey,1<<12,0600|IPC_CREAT);
	if(-1 == shmid){
		perror("shmget");
		return -1;
	}
	char *p;
	p = (char*)shmat(shmid,NULL,0);
	if((char*)-1 == p){
		perror("shmat");
		return -1;
	}

	bzero(p,sizeof(p));
	//printf("the p is %s\n",p);

	int ret;
	//信号量
	int semid;
	semid = semget(skey,2,0600|IPC_CREAT);
	unsigned short arr[2];
	arr[0] = 1;
	arr[1] = 0;
	ret = semctl(semid,0,SETALL,arr);
	if(-1 == ret){
		perror("semctl");
		return -1;
	}
	struct sembuf sop;

	bzero(&sop,sizeof(sop));
	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = SEM_UNDO;

	struct sembuf sov;
	bzero(&sov,sizeof(sov));
	sov.sem_num = 0;
	sov.sem_op = 1;
	sov.sem_flg = SEM_UNDO;

	struct sembuf produce;
	bzero(&produce,sizeof(produce));
	produce.sem_num = 1;
	produce.sem_op = 1;
	produce.sem_flg = SEM_UNDO;
	

	char buf[128];
	fd_set readset;
	while(!qflag)
	{
		FD_ZERO(&readset); 
		FD_SET(STDIN_FILENO,&readset);
		FD_SET(fdr,&readset);
		ret=select(fdr+1,&readset,NULL,NULL,NULL);
		if(ret >0)
		{
			if(FD_ISSET(fdr,&readset)) //如果fdr可读
			{
				
				bzero(buf,sizeof(buf));
				if(read(fdr,buf,sizeof(buf))>0)
				{
					//ret = semctl(semid,0,GETVAL);
					//printf("before the ret is %d\n",ret);
					semop(semid,&sop,1);
					//ret = semctl(semid,0,GETVAL);
					//printf("in sop %d\n",ret);
					

					memcpy(p,buf,sizeof(buf));
					//printf("the p id %s\n",p);
					semop(semid,&produce,1);

					semop(semid,&sov,1);
					//ret = semctl(semid,0,GETVAL);
					//printf("sov the ret is %d\n",ret);
					//ret = semctl(semid,1,GETVAL);
					//printf("procude the ret is %d\n",ret);
					

//					printf("%s\n",buf);
				}
			}
			if(FD_ISSET(STDIN_FILENO,&readset))//如果标准输入可读
			{
				
				bzero(buf,sizeof(buf));
				if((ret=read(STDIN_FILENO,buf,sizeof(buf)))>0)
				{	
					write(fdw,buf,ret-1);
					semop(semid,&sop,1);
					//ret = semctl(semid,0,GETVAL);
					//printf("in sop %d\n",ret);
					

					memcpy(p,buf,ret - 1);
					//printf("the p id %s\n",p);
					semop(semid,&produce,1);

					semop(semid,&sov,1);
					//ret = semctl(semid,0,GETVAL);

				}else{
					write(fdw,"bye",3);
					break;
				}
			}				
		}
	}
	close(fdr);
	close(fdw);
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}
