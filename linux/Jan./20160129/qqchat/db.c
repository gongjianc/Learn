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
#include <string.h>
#include <sys/shm.h>

#define PROJ_ID 1

int main(int argc,char* argv[])
{
	if(argc !=2)
	{
		printf("error args\n");
		return -1;
	}

	key_t skey;
	skey = ftok(argv[1],PROJ_ID);
	if(-1 == skey){
		perror("ftok");
		return -1;
	}

	int shmid;
	shmid = shmget(skey,1<<12,0600|IPC_CREAT);
	if(-1 == shmid){
		perror("shmget");
		return -1;
	}
//	printf("shmid = %d\n",shmid);
	char *p;
	p = (char*)shmat(shmid,NULL,0);

//	printf("after p\n");

	if((char*)-1 == p){
		perror("shmat");
		return -1;
	}
//	printf("p is %s\n",p);

	int ret;
	int semid;
	semid = semget(skey,2,0600|IPC_CREAT);

	unsigned short arr[2];
	ret = semctl(semid,0,GETALL,arr);


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

	struct sembuf consum;
	bzero(&consum,sizeof(consum));
	consum.sem_num = 1;
	consum.sem_op = -1;
	consum.sem_flg = SEM_UNDO;

	char buf[128];
	fd_set readset;
	while(1)
	{
		if(*p != 0){
			semop(semid,&sop,1);
//			printf("in sop\n");
			semop(semid,&consum,1);
//			printf("in consum\n");
//			printf("p is %s\n",p);
			bzero(buf,sizeof(buf));
//			printf("the strlen p is %d\n",(int)strlen(p));
//			printf("the sizeof p is %d\n",(int)sizeof(p));
			memcpy(buf,p,strlen(p));
			printf("%s\n",buf);
//			printf("before sov\n");
			bzero(p,strlen(p));
			semop(semid,&sov,1);
//			printf("out sov\n");
			
		}
	}
	return 0;
}
