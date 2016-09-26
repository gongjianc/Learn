#include "msgqueue.h"

int receive_msg(struct file_msg *arg) {
	int msgid;
	memset(arg, 0, sizeof(struct file_msg));
	// 获得消息队列ID
	msgid=msgget(WLTOCAP_KEY,0666| IPC_CREAT);
	if (msgid == -1) {
		return -1;
	}
	arg->mtype = 1;
	int res = msgrcv(msgid, arg, sizeof(struct file_msg), 0,0);
	printf("\n\n\n___________res = %d_____________\n\n\n",res);
	if (-1 == res){
		return -1;
	}
	return 1;
}
