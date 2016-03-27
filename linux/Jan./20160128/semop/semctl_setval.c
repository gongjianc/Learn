#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/sem.h>

int main(int argc,char* argv[])
{
	int semid;
	semid = semget((key_t)1234,1,0600|IPC_CREAT);
	if(-1 == semid){
		perror("semget");
		return -1;
	}
	printf("semid is %d \n",semid);

	int ret;
	ret = semctl(semid,0,SETVAL,1);
	if(-1 == ret){
		perror("semctl");
		return -1;
	}
	printf("after set , semid = %d \n",ret);
	return 0;
}
