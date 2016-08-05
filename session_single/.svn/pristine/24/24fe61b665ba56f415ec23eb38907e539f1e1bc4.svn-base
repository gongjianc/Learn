#ifndef PFCOUNTWL_H_
#define PFCOUNTWL_H_

#define ENABLE_BPF 1

typedef  struct Lins{
	json_t *HashMapLinsSips;
	json_t * HashMapLinsDips;
	int DipsFlag;
	int SipsFlag;
}LinsRules;


typedef struct info{
	json_t *JsonRoot;
	json_t *Lins;
	json_t *Wans;
	int WansFlag;
	json_t  *Lans;
	int LansFlag;
	LinsRules *LinsIp;
	LinsRules LinsFlag;
	int LinSize;
	int use_flag;
	int up_flag;
	int shmid;
}FilterInfo;


int shmid1;
char *msg;
#define SHMID_SIZE 33554432

#define SHMID1 0x1024
#define SHMID2 0x2048
void *ReadShareMemory(void);
int compareIP(char *sips, char *dips); 

#endif /* PFCOUNTWL_H_ */
