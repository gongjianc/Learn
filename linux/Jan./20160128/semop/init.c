#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/sem.h>

#define PROJ_ID 1

int
main(int argc,char* argv[])
{
	if(argc != 2){
		printf("error args\n");
		return -1;
	}
	key_t skey;
	skey = ftok(argv[1],PROJ_ID);
	if(-1 == skey){
		perror("ftok");
		return -1;
	}
	printf("the key is %d\n",skey);
	int shmid;
	shmid = shmget(skey,1<<12,0600 | IPC_CREAT);
	if(-1 == shmid){
		perror("shmget");
		return -1;
	}
	printf("the shmid is %d\n",shmid);
	int *p;
	p = (int*)shmat(shmid,NULL,0);
	if((int*)-1==p){
		perror("shmat");
		return -1;
	}
	*p = 0;
	printf("the value is %d\n",*p);
	return 0;
}
