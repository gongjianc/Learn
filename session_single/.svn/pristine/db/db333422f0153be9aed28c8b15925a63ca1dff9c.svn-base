#ifndef MSGQUEUE_H_
#define MSGQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>

#define WL_JSONLEN 256 

// wl to capture
#define WLTOCAP_KEY (key_t)0x27db
//#define WLTOCAP_KEY (key_t)0x1111
//#define WLTOCAP_KEY (key_t)0x27dc

struct file_msg {
	long mtype; /* message type, must be > 0 */
	char mtext[WL_JSONLEN]; /* message data */
};

int receive_msg(struct file_msg *arg);

#endif /* MSGQUEUE_H_ */
